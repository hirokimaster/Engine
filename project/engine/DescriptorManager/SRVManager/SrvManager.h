#pragma once
#include "engine/DescriptorManager/DescriptorManager.h"
#include "engine/CreateResource/CreateResource.h"
#include "engine/Model/Animation/Animation.h"
#include <queue>

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
	/// 空いてる番号を取り出す
	/// </summary>
	void Allocate();

	/// <summary>
	/// 使ってないインデックス解放する
	/// </summary>
	void Free(uint32_t index);

#pragma region getter

	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle(uint32_t texHandle);
	uint32_t GetIndex() { return index_; }

#pragma endregion

#pragma region setter

    uint32_t SetIndex(uint32_t index) { return index_ = index; }

	void SetGraphicsRootDescriptorTable(UINT rootParameterIndex, uint32_t srvIndex);

#pragma endregion

private:
	SrvManager() = default;
	~SrvManager() = default;
	SrvManager(const SrvManager&) = delete;
	SrvManager& operator=(const SrvManager&) = delete;

private:
	// 最大SRV数
	static const uint32_t kMaxSRVCount = 1024;

	D3D12_CPU_DESCRIPTOR_HANDLE cpuDescHandle_[kMaxSRVCount];
	D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandle_[kMaxSRVCount];

	uint32_t index_ = 0; // srvのインデックス
	uint32_t beforeIndex_ = 0; // 前のインデックスを入れとく
	std::queue<uint32_t> vacantIndices_; // 空きインデックスを管理
};
