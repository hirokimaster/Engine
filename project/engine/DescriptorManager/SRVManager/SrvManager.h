#pragma once
#include "engine/DescriptorManager/DescriptorManager.h"
#include "engine/CreateResource/CreateResource.h"
#include "engine/Model/Animation/Animation.h"
#define MAX_SRV 128

class SrvManager {
public:

	/// <summary>
	/// シングルトンインスタンス
	/// </summary>
	/// <returns></returns>
	static SrvManager* GetInstance();

	/// <summary>
	/// textureのsrv
	/// </summary>
	/// <param name="resource"></param>
	/// <param name="metadata"></param>
	/// <param name="index"></param>
	void CreateTextureSrv(Microsoft::WRL::ComPtr<ID3D12Resource> resource, const DirectX::TexMetadata& metadata, uint32_t index);

	/// <summary>
	/// instancing用のsrv
	/// </summary>
	/// <param name="resource"></param>
	/// <param name="metadata"></param>
	/// <param name="index"></param>
	void CreateInstancingSrv(Resource& resource,uint32_t index);

	/// <summary>
	/// postProcess用のsrv
	/// </summary>
	/// <param name="resource"></param>
	/// <param name="index"></param>
	void CreatePostProcessSrv(Microsoft::WRL::ComPtr<ID3D12Resource> resource, uint32_t index);
	
	/// <summary>
	/// depthTextureSRV
	/// </summary>
	/// <param name="resource"></param>
	/// <param name="index"></param>
	void CreateDepthTextureSrv(Microsoft::WRL::ComPtr<ID3D12Resource> resource, uint32_t index);

	/// <summary>
	///  animationのpalette用のsrv
	/// </summary>
	/// <param name="skinCluster"></param>
	/// <param name="skeleton"></param>
	//void CreatePaletteSrv(SkinCluster& skinCluster,Skeleton& skeleton, uint32_t index);

	/// <summary>
	/// srvのgpuhandleの位置をずらす
	/// </summary>
	void ShiftIndex();

#pragma region getter

	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle(uint32_t texHandle);
	D3D12_GPU_DESCRIPTOR_HANDLE GetInstancingGPUHandle(uint32_t texHandle);
	uint32_t GetIndex() { return index_; }

#pragma endregion

#pragma region setter

    uint32_t SetIndex(uint32_t index) { return index_ = index; }

#pragma endregion

private:
	SrvManager() = default;
	~SrvManager() = default;
	SrvManager(const SrvManager&) = delete;
	SrvManager& operator=(const SrvManager&) = delete;

private:

	D3D12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV_[MAX_SRV];
	D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV_[MAX_SRV];
	D3D12_CPU_DESCRIPTOR_HANDLE instancingSrvHandleCPU_[MAX_SRV];
	D3D12_GPU_DESCRIPTOR_HANDLE instancingSrvHandleGPU_[MAX_SRV];
	uint32_t index_;

};
