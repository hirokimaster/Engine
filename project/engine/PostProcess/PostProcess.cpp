#include "PostProcess.h"

PostProcess::PostProcess()
{
}

PostProcess::~PostProcess()
{
}

void PostProcess::Initialize()
{
	worldTransform_.Initialize();
	camera_.Initialize();
	// srvを作るところを今使ってるところの隣にずらす
	SrvManager::GetInstance()->ShiftIndex();
	index_ = SrvManager::GetInstance()->GetIndex();
	CreateSRV();
	CreateRTV();
	CreateDSV();
	CreateVertex();

	// クライアント領域のサイズと一緒にして画面全体に表示
	viewport.Width = 1280;
	viewport.Height = 720;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	// 基本的にビューポートと同じ矩形が構成されるようにする
	scissorRect.left = 0;
	scissorRect.right = 1280;
	scissorRect.top = 0;
	scissorRect.bottom = 720;

	bloom_ = CreateResource::CreateBufferResource(sizeof(Bloom));
	bloom_->Map(0, nullptr, reinterpret_cast<void**>(&bloomData_));
	bloomData_->stepWeight = 0.001f;
	bloomData_->sigma = 0.005f;
	bloomData_->lightStrength = 1.0;
	bloomData_->bloomThreshold = 0.5f;

}

void PostProcess::CreateVertex()
{
#pragma region // 頂点データ

	resource_.vertexResource = CreateResource::CreateBufferResource(sizeof(VertexData) * 6);

	VBV_ = CreateResource::CreateVertexBufferView(resource_.vertexResource, sizeof(VertexData) * 6, 6);

	// 頂点データの設定
	VertexData* vertexDataSprite = nullptr;
	resource_.vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexDataSprite));

	// 1枚目の三角形
	vertexDataSprite[0].position = { 0.0f, 720.0f,0.0f, 1.0f }; // 左下
	vertexDataSprite[0].texcoord = { 0.0f, 1.0f };
	vertexDataSprite[1].position = { 0.0f, 0.0f, 0.0f, 1.0f }; // 左上
	vertexDataSprite[1].texcoord = { 0.0f, 0.0f };
	vertexDataSprite[2].position = { 1280.0f, 720.0f, 0.0f,1.0f }; // 右下
	vertexDataSprite[2].texcoord = { 1.0f,1.0f };
	// 2枚目の三角形
	vertexDataSprite[3].position = { 0.0f, 0.0f, 0.0f, 1.0f }; // 左上
	vertexDataSprite[3].texcoord = { 0.0f, 0.0f };
	vertexDataSprite[4].position = { 1280.0f, 0.0f, 0.0f, 1.0f }; // 右上
	vertexDataSprite[4].texcoord = { 1.0f, 0.0f };
	vertexDataSprite[5].position = { 1280.0f, 720.0f, 0.0f,1.0f }; // 右下
	vertexDataSprite[5].texcoord = { 1.0f,1.0f };

#pragma endregion

	resource_.wvpResource = CreateResource::CreateBufferResource(sizeof(TransformationMatrix));

}

void PostProcess::CreateRTV()
{
	// RTVの設定
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	rtvHandles_ = DescriptorManager::GetInstance()->GetCPUDescriptorHandle(DescriptorManager::GetInstance()->GetRTV(), DescriptorManager::GetInstance()->GetDescSize().RTV, 2);
	DirectXCommon::GetInstance()->GetDevice()->CreateRenderTargetView(texBuff_.Get(), &rtvDesc, rtvHandles_);
}

void PostProcess::CreateDSV()
{
	// 生成するResourceの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = 1280; // Textureの幅
	resourceDesc.Height = 720; // Textureの高さ
	resourceDesc.MipLevels = 1; // mipmapの数
	resourceDesc.DepthOrArraySize = 1; // 奥行 or 配列Textureの配列数
	resourceDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // DepthStencilとして使う通知
	resourceDesc.SampleDesc.Count = 1; // サンプリングカウント。1固定
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D; // 2次元
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL; // DepthStencilとして使う通知

	// 利用するHeapの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT; // VRAM上に作る

	// 深度値のクリア設定
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f; // 1.0f（最大値）でクリア
	depthClearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // フォーマット。Resourceと合わせる

	// Resourceの生成
	HRESULT hr = DirectXCommon::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProperties, // Heapの設定
		D3D12_HEAP_FLAG_NONE, // Heapの特殊な設定。特になし
		&resourceDesc, // Resourceの設定
		D3D12_RESOURCE_STATE_DEPTH_WRITE, // 深度値を書き込む状態にしておく
		&depthClearValue, // Clear最適値
		IID_PPV_ARGS(&depthBuffer_)); // 作成するResourceポインタへのポインタ
	assert(SUCCEEDED(hr));

	// DSVの設定
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // Format。基本的にはResourceに合わせる
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D; // 2dTexture
	// 2番目に作る
	dsvHandles_ = DescriptorManager::GetInstance()->GetCPUDescriptorHandle(DescriptorManager::GetInstance()->GetDSV(), DescriptorManager::GetInstance()->GetDescSize().DSV, 1);
	DirectXCommon::GetInstance()->GetDevice()->CreateDepthStencilView(depthBuffer_.Get(), &dsvDesc, dsvHandles_);


}

void PostProcess::CreateSRV()
{
	Microsoft::WRL::ComPtr<ID3D12Device> device = DirectXCommon::GetInstance()->GetDevice();

	// metadataを基にResourceの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = 1280; // Textureの幅
	resourceDesc.Height = 720; // Textureの高さ
	resourceDesc.MipLevels = 1; // mipmapの数
	resourceDesc.DepthOrArraySize = 1; // 奥行 or 配列Textureの配列数
	resourceDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // TextureのFormat
	resourceDesc.SampleDesc.Count = 1; // サンプリングカウント。1固定 
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
	// 利用するHeapの設定。非常に特殊な運用。02_04exで一般的なケース版がある
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_CUSTOM; // 細かい設定を行う
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK; // WriteBackポリシーでCPUアクセス可能
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_L0; // プロセッサの近くに配置
	D3D12_CLEAR_VALUE clearValue{};
	float clearColor[] = { 0.1f, 0.25f, 0.5f, 1.0f };
	clearValue.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	clearValue.Color[0] = clearColor[0];
	clearValue.Color[1] = clearColor[1];
	clearValue.Color[2] = clearColor[2];
	clearValue.Color[3] = clearColor[3];
	// Resourceの作成
	HRESULT hr = device->CreateCommittedResource(
		&heapProperties, // Heapの設定
		D3D12_HEAP_FLAG_NONE, // Heapの特殊な設定。特になし。
		&resourceDesc, // Resourceの設定
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		&clearValue, // Clear最適値。使わないのでnullptr
		IID_PPV_ARGS(&texBuff_)); // 作成するResourceポインタへのポインタ
	assert(SUCCEEDED(hr));

	SrvManager::GetInstance()->CreatePostProcessSrv(texBuff_.Get(), index_);

}

void PostProcess::PreDraw()
{
	barrier_.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	// Noneにしておく
	barrier_.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	// バリアを張る対象のリソース
	barrier_.Transition.pResource = texBuff_.Get();
	// 遷移前（現在）のResourceState
	barrier_.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	// 遷移後のResourceState
	barrier_.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	DirectXCommon::GetInstance()->GetCommandList()->ResourceBarrier(1, &barrier_);

	// レンダーターゲットをセット
	DirectXCommon::GetInstance()->GetCommandList()->OMSetRenderTargets(1, &rtvHandles_, false, &dsvHandles_);
	// viewport
	DirectXCommon::GetInstance()->GetCommandList()->RSSetViewports(1, &viewport);
	// scissorRect
	DirectXCommon::GetInstance()->GetCommandList()->RSSetScissorRects(1, &scissorRect);
	// 全画面クリア
	float clearColor[] = { 0.1f, 0.25f, 0.5f, 1.0f };
	DirectXCommon::GetInstance()->GetCommandList()->ClearRenderTargetView(rtvHandles_, clearColor, 0, nullptr);
	// 深度バッファのクリア
	DirectXCommon::GetInstance()->GetCommandList()->ClearDepthStencilView(dsvHandles_, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	ID3D12DescriptorHeap* heaps[] = { DescriptorManager::GetInstance()->GetSRV() };
	DirectXCommon::GetCommandList()->SetDescriptorHeaps(_countof(heaps), heaps);
}

void PostProcess::PostDraw()
{
	barrier_.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier_.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	DirectXCommon::GetInstance()->GetCommandList()->ResourceBarrier(1, &barrier_);
}

void PostProcess::Draw()	
{
	camera_.UpdateMatrix();
	//worldTransform_.STransferMatrix(resource_.wvpResource, camera_);

	Property property = GraphicsPipeline::GetInstance()->GetPSO().PostEffectBloom;

	// Rootsignatureを設定。PSOに設定してるけど別途設定が必要
	DirectXCommon::GetCommandList()->SetGraphicsRootSignature(property.rootSignature_.Get());
	DirectXCommon::GetCommandList()->SetPipelineState(property.graphicsPipelineState_.Get()); // PSOを設定
	DirectXCommon::GetCommandList()->IASetVertexBuffers(0, 1, &VBV_); // VBVを設定
	// 形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけば良い
	DirectXCommon::GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// wvp用のCBufferの場所を設定
	DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(0, resource_.wvpResource->GetGPUVirtualAddress());
	DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(1, bloom_->GetGPUVirtualAddress());
	DirectXCommon::GetCommandList()->SetGraphicsRootDescriptorTable(2, SrvManager::GetInstance()->GetGPUHandle(index_));
	// 描画。(DrawCall/ドローコール)。3頂点で1つのインスタンス。インスタンスについては今後
	DirectXCommon::GetCommandList()->DrawInstanced(6, 1, 0, 0);
}
