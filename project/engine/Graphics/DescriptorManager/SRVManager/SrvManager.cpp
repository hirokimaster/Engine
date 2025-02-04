/**
* @file SrvManager.cpp
* @brief シェーダーリソースビュー管理マネージャ
* @author 仁平 琉乃
*/

#include "SrvManager.h"

SrvManager* SrvManager::GetInstance()
{
	static SrvManager instance;
	return &instance;
}

void SrvManager::CreateTextureSrv(Microsoft::WRL::ComPtr<ID3D12Resource> resource, const DirectX::TexMetadata& metadata, uint32_t index)
{
	// metaDataを基にSRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	// cubeMapかどうか判定する
	if (metadata.IsCubemap()) {
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
		srvDesc.TextureCube.MostDetailedMip = 0;
		srvDesc.TextureCube.MipLevels = UINT_MAX;
		srvDesc.TextureCube.ResourceMinLODClamp = 0.0f;
	}
	else {
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; // 2Dテクスチャ
		srvDesc.Texture2D.MipLevels = UINT(metadata.mipLevels);
	}
	
	// SRVを作成するDescriptorHeapの場所を決める
	cpuDescHandle_[index] = DescriptorManager::GetInstance()->GetCPUDescriptorHandle(DescriptorManager::GetInstance()->GetSRV(), DescriptorManager::GetInstance()->GetDescSize().SRV, index);
	gpuDescHandle_[index] = DescriptorManager::GetInstance()->GetGPUDescriptorHandle(DescriptorManager::GetInstance()->GetSRV(), DescriptorManager::GetInstance()->GetDescSize().SRV, index);

	// SRVの生成
	DirectXCommon::GetInstance()->GetDevice()->CreateShaderResourceView(resource.Get(), &srvDesc, cpuDescHandle_[index]);
}

void SrvManager::CreateInstancingSrv(Microsoft::WRL::ComPtr<ID3D12Resource> resource, uint32_t index)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC instancingSrvDesc{};
	instancingSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
	instancingSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	instancingSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	instancingSrvDesc.Buffer.FirstElement = 0;
	instancingSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	instancingSrvDesc.Buffer.NumElements = 100;
	instancingSrvDesc.Buffer.StructureByteStride = sizeof(ParticleForGPU);
	cpuDescHandle_[index] = DescriptorManager::GetCPUDescriptorHandle(DescriptorManager::GetInstance()->GetSRV(), DescriptorManager::GetInstance()->GetDescSize().SRV, index);
	gpuDescHandle_[index] = DescriptorManager::GetGPUDescriptorHandle(DescriptorManager::GetInstance()->GetSRV(), DescriptorManager::GetInstance()->GetDescSize().SRV, index);
	DirectXCommon::GetInstance()->GetDevice()->CreateShaderResourceView(resource.Get(), &instancingSrvDesc, cpuDescHandle_[index]);
}

void SrvManager::CreateStructuredBufferSrv(Microsoft::WRL::ComPtr<ID3D12Resource> resource, uint32_t num, uint32_t size, uint32_t index)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC instancingSrvDesc{};
	instancingSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
	instancingSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	instancingSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	instancingSrvDesc.Buffer.FirstElement = 0;
	instancingSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	instancingSrvDesc.Buffer.NumElements = num;
	instancingSrvDesc.Buffer.StructureByteStride = UINT(size);
	cpuDescHandle_[index] = DescriptorManager::GetCPUDescriptorHandle(DescriptorManager::GetInstance()->GetSRV(), DescriptorManager::GetInstance()->GetDescSize().SRV, index);
	gpuDescHandle_[index] = DescriptorManager::GetGPUDescriptorHandle(DescriptorManager::GetInstance()->GetSRV(), DescriptorManager::GetInstance()->GetDescSize().SRV, index);
	DirectXCommon::GetInstance()->GetDevice()->CreateShaderResourceView(resource.Get(), &instancingSrvDesc, cpuDescHandle_[index]);
}

void SrvManager::CreatePostProcessSrv(Microsoft::WRL::ComPtr<ID3D12Resource> resource, uint32_t index)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; // 2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;

	cpuDescHandle_[index] = DescriptorManager::GetInstance()->GetCPUDescriptorHandle(DescriptorManager::GetInstance()->GetSRV(), DescriptorManager::GetInstance()->GetDescSize().SRV, index);
	gpuDescHandle_[index] = DescriptorManager::GetInstance()->GetGPUDescriptorHandle(DescriptorManager::GetInstance()->GetSRV(), DescriptorManager::GetInstance()->GetDescSize().SRV, index);
	DirectXCommon::GetInstance()->GetDevice()->CreateShaderResourceView(resource.Get(), &srvDesc, cpuDescHandle_[index]);
}

void SrvManager::CreateDepthTextureSrv(Microsoft::WRL::ComPtr<ID3D12Resource> resource, uint32_t index)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;	
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	cpuDescHandle_[index] = DescriptorManager::GetInstance()->GetCPUDescriptorHandle(DescriptorManager::GetInstance()->GetSRV(), DescriptorManager::GetInstance()->GetDescSize().SRV, index);
	gpuDescHandle_[index] = DescriptorManager::GetInstance()->GetGPUDescriptorHandle(DescriptorManager::GetInstance()->GetSRV(), DescriptorManager::GetInstance()->GetDescSize().SRV, index);
	DirectXCommon::GetInstance()->GetDevice()->CreateShaderResourceView(resource.Get(), &srvDesc, cpuDescHandle_[index]);
}

void SrvManager::Allocate()
{
	// 空き番号があるか確認(あったらそこから使う)
	if (!vacantIndices_.empty()) {
		index_ = vacantIndices_.front(); // 先頭から取る
		vacantIndices_.pop(); // 使うのでコンテナから削除する
	}
	else {
		++beforeIndex_;
		index_ = beforeIndex_;
	}
}

void SrvManager::Free(uint32_t index)
{
	vacantIndices_.push(index);
}

void SrvManager::StructuredBufIndexAllocate()
{
	// 空き番号があるか確認(あったらそこから使う)
	if (!vacantStructuredBufIndices_.empty()) {
		structuredBufIndex_ = vacantStructuredBufIndices_.front(); // 先頭から取る
		vacantStructuredBufIndices_.pop(); // 使うのでコンテナから削除する
	}
	else {
		++beforestructuredBufIndex_;
		// 最大数を超えたら止める
		//assert(beforestructuredBufIndex_ > kMaxSRVCount);
		structuredBufIndex_ = beforestructuredBufIndex_;
	}
}

void SrvManager::StructuredBufIndexFree(uint32_t index)
{
	vacantStructuredBufIndices_.push(index);
}

void SrvManager::UavAllocate()
{
	// 空き番号があるか確認(あったらそこから使う)
	if (!vacantUavIndices_.empty()) {
		uavIndex_ = vacantUavIndices_.front(); // 先頭から取る
		vacantUavIndices_.pop(); // 使うのでコンテナから削除する
	}
	else {
		++beforeUavIndex_;
		uavIndex_ = beforeUavIndex_;
	}
}

void SrvManager::UavFree(uint32_t index)
{
	vacantUavIndices_.push(index);
}

D3D12_GPU_DESCRIPTOR_HANDLE SrvManager::GetGPUHandle(uint32_t texHandle)
{
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = gpuDescHandle_[texHandle];

	return handleGPU;
}

void SrvManager::SetGraphicsRootDescriptorTable(UINT rootParameterIndex, uint32_t srvIndex)
{
	DirectXCommon::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(rootParameterIndex, GetGPUHandle(srvIndex));
}

