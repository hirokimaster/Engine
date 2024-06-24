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
	cpuDescHandleSRV_[index] = DescriptorManager::GetInstance()->GetCPUDescriptorHandle(DescriptorManager::GetInstance()->GetSRV(), DescriptorManager::GetInstance()->GetDescSize().SRV, index + 1);
	gpuDescHandleSRV_[index] = DescriptorManager::GetInstance()->GetGPUDescriptorHandle(DescriptorManager::GetInstance()->GetSRV(), DescriptorManager::GetInstance()->GetDescSize().SRV, index + 1);

	// SRVの生成
	DirectXCommon::GetInstance()->GetDevice()->CreateShaderResourceView(resource.Get(), &srvDesc, cpuDescHandleSRV_[index]);
}

void SrvManager::CreateInstancingSrv(Resource& resource, uint32_t index)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC instancingSrvDesc{};
	instancingSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
	instancingSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	instancingSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	instancingSrvDesc.Buffer.FirstElement = 0;
	instancingSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	instancingSrvDesc.Buffer.NumElements = 100;
	instancingSrvDesc.Buffer.StructureByteStride = sizeof(ParticleForGPU);
	instancingSrvHandleCPU_[index] = DescriptorManager::GetCPUDescriptorHandle(DescriptorManager::GetInstance()->GetSRV(), DescriptorManager::GetInstance()->GetDescSize().SRV, index + 1);
	instancingSrvHandleGPU_[index] = DescriptorManager::GetGPUDescriptorHandle(DescriptorManager::GetInstance()->GetSRV(), DescriptorManager::GetInstance()->GetDescSize().SRV, index + 1);
	DirectXCommon::GetInstance()->GetDevice()->CreateShaderResourceView(resource.instancingResource.Get(), &instancingSrvDesc, instancingSrvHandleCPU_[index]);
}

void SrvManager::CreatePostProcessSrv(Microsoft::WRL::ComPtr<ID3D12Resource> resource, uint32_t index)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; // 2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;

	cpuDescHandleSRV_[index] = DescriptorManager::GetInstance()->GetCPUDescriptorHandle(DescriptorManager::GetInstance()->GetSRV(), DescriptorManager::GetInstance()->GetDescSize().SRV, index);
	gpuDescHandleSRV_[index] = DescriptorManager::GetInstance()->GetGPUDescriptorHandle(DescriptorManager::GetInstance()->GetSRV(), DescriptorManager::GetInstance()->GetDescSize().SRV, index);
	DirectXCommon::GetInstance()->GetDevice()->CreateShaderResourceView(resource.Get(), &srvDesc, cpuDescHandleSRV_[index]);
}

void SrvManager::CreateDepthTextureSrv(Microsoft::WRL::ComPtr<ID3D12Resource> resource, uint32_t index)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;	
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	cpuDescHandleSRV_[index] = DescriptorManager::GetInstance()->GetCPUDescriptorHandle(DescriptorManager::GetInstance()->GetSRV(), DescriptorManager::GetInstance()->GetDescSize().SRV, index);
	gpuDescHandleSRV_[index] = DescriptorManager::GetInstance()->GetGPUDescriptorHandle(DescriptorManager::GetInstance()->GetSRV(), DescriptorManager::GetInstance()->GetDescSize().SRV, index);
	DirectXCommon::GetInstance()->GetDevice()->CreateShaderResourceView(resource.Get(), &srvDesc, cpuDescHandleSRV_[index]);
}

//void SrvManager::CreatePaletteSrv(SkinCluster& skinCluster, Skeleton& skeleton, uint32_t index)
//{
//	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
//	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
//	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
//	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
//	srvDesc.Buffer.FirstElement = 0;
//	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
//	srvDesc.Buffer.NumElements = UINT(skeleton.joints.size());
//	srvDesc.Buffer.StructureByteStride = sizeof(WellForGPU);
//	cpuDescHandleSRV_[index] = DescriptorManager::GetCPUDescriptorHandle(DescriptorManager::GetInstance()->GetSRV(), DescriptorManager::GetInstance()->GetDescSize().SRV, index);
//	gpuDescHandleSRV_[index] = DescriptorManager::GetGPUDescriptorHandle(DescriptorManager::GetInstance()->GetSRV(), DescriptorManager::GetInstance()->GetDescSize().SRV, index);
//	skinCluster.paletteSrvHandle.first = cpuDescHandleSRV_[index];
//	skinCluster.paletteSrvHandle.second = gpuDescHandleSRV_[index];
//	DirectXCommon::GetInstance()->GetDevice()->CreateShaderResourceView(skinCluster.paletteResource.Get(), &srvDesc, skinCluster.paletteSrvHandle.first);
//}

void SrvManager::ShiftIndex()
{
	++index_;
}

D3D12_GPU_DESCRIPTOR_HANDLE SrvManager::GetGPUHandle(uint32_t texHandle)
{
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = gpuDescHandleSRV_[texHandle];

	return handleGPU;
}

D3D12_GPU_DESCRIPTOR_HANDLE SrvManager::GetInstancingGPUHandle(uint32_t texHandle)
{
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = instancingSrvHandleGPU_[texHandle];

	return handleGPU;
}

