#include "TextureManager.h"

TextureManager* TextureManager::GetInstance() {
	static TextureManager instance;
	return &instance;
}

uint32_t TextureManager::Load(const std::string& fileName) {

	auto it = TextureManager::GetInstance()->fileHandleMap.find(fileName);
	if (it != TextureManager::GetInstance()->fileHandleMap.end()) {
		// 既にロードされたファイルの場合、そのハンドルを返す
		return it->second;
	}

	// 新しいのならindexをずらして新しく作る
	uint32_t newIndex = SrvManager::GetInstance()->GetIndex();
	newIndex++;
	SrvManager::GetInstance()->ShiftIndex();
	TextureManager::GetInstance()->fileHandleMap[fileName] = newIndex;
	LoadTexture(fileName, newIndex);

	return newIndex;
}


DirectX::ScratchImage TextureManager::LoadTexture(const std::string& filePath) {
	// テクスチャファイルを読んでプログラムで扱えるようにする
	DirectX::ScratchImage image{};
	std::wstring filePathW = ConvertString(filePath);
	HRESULT hr{};
	if (filePathW.ends_with(L".dds")) {
		hr = DirectX::LoadFromDDSFile(filePathW.c_str(), DirectX::DDS_FLAGS_NONE, nullptr, image);
	}
	else {
		hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
	}
	
	assert(SUCCEEDED(hr));

	// ミニマップの作成
	DirectX::ScratchImage mipImages{};

	if (DirectX::IsCompressed(image.GetMetadata().format)) {
		mipImages = std::move(image);
	}
	else {
		hr = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 4, mipImages);
	}
	
	assert(SUCCEEDED(hr));

	// ミニマップ付きのデータを返す
	return mipImages;
}

void TextureManager::LoadTexture(const std::string& filePath, uint32_t index)
{

	index = SrvManager::GetInstance()->GetIndex();
	DirectX::ScratchImage mipImages = LoadTexture(filePath);
	TextureManager::GetInstance()->metadata_[index] = mipImages.GetMetadata();

	// SRV作成
	TextureManager::GetInstance()->texResource[index] = CreateTextureResource(TextureManager::GetInstance()->metadata_[index]);
	TextureManager::GetInstance()->intermediateResource_ = UploadTextureData(TextureManager::GetInstance()->texResource[index].Get(), mipImages);
	SrvManager::GetInstance()->CreateTextureSrv(TextureManager::GetInstance()->texResource[index], TextureManager::GetInstance()->metadata_[index], index);
}


ID3D12Resource* TextureManager::CreateTextureResource(const DirectX::TexMetadata& metadata) {

	Microsoft::WRL::ComPtr<ID3D12Device> device = DirectXCommon::GetInstance()->GetDevice();

	// metadataを基にResourceの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = UINT(metadata.width); // Textureの幅
	resourceDesc.Height = UINT(metadata.height); // Textureの高さ
	resourceDesc.MipLevels = UINT16(metadata.mipLevels); // mipmapの数
	resourceDesc.DepthOrArraySize = UINT16(metadata.arraySize); // 奥行 or 配列Textureの配列数
	resourceDesc.Format = metadata.format; // TextureのFormat
	resourceDesc.SampleDesc.Count = 1; // サンプリングカウント。1固定
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION(metadata.dimension);  // Textureの次元数。普段使っているのは2次元
	// 利用するHeapの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT; // default
	//heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK; // WriteBackポリシーでCPUアクセス可能
	//heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_L0; // プロセッサの近くに配置
	// Resourceの作成
	ID3D12Resource* resource = nullptr;
	HRESULT hr = device->CreateCommittedResource(
		&heapProperties, // Heapの設定
		D3D12_HEAP_FLAG_NONE, // Heapの特殊な設定。特になし。
		&resourceDesc, // Resourceの設定
		D3D12_RESOURCE_STATE_COPY_DEST, // データ転送される設定
		nullptr, // Clear最適値。使わないのでnullptr
		IID_PPV_ARGS(&resource)); // 作成するResourceポインタへのポインタ
	assert(SUCCEEDED(hr));

	return resource;

}

[[nodiscard]]
ID3D12Resource* TextureManager::UploadTextureData(Microsoft::WRL::ComPtr<ID3D12Resource> texture, const DirectX::ScratchImage& mipImages) {

	std::vector<D3D12_SUBRESOURCE_DATA> subresource;
	DirectX::PrepareUpload(DirectXCommon::GetInstance()->GetDevice(), mipImages.GetImages(), mipImages.GetImageCount(), mipImages.GetMetadata(), subresource);
	uint64_t intermediateSize = GetRequiredIntermediateSize(texture.Get(), 0, UINT(subresource.size()));
	ID3D12Resource* intermediateResource = CreateResource::CreateIntermediateResource(intermediateSize);
	UpdateSubresources(DirectXCommon::GetCommandList(), texture.Get(), intermediateResource, 0, 0, UINT(subresource.size()), subresource.data());
	// texture転送後は利用できるよう、STATE_COPY_DESTからSTATE_GENERIC_READへ変更する
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = texture.Get();
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;
	DirectXCommon::GetCommandList()->ResourceBarrier(1, &barrier);

	return intermediateResource;
}

const DirectX::TexMetadata& TextureManager::GetMetaData(uint32_t textureIndex)
{
	
	return metadata_[textureIndex];
}

void TextureManager::Release()
{
	TextureManager::GetInstance()->intermediateResource_->Release();
}
