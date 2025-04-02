/**
* @file SrvManager.h
* @brief シェーダーリソースビュー管理マネージャ
* @author 仁平 琉乃
*/

#pragma once
#include "engine/Graphics/DescriptorManager/DescriptorManager.h"
#include "engine/Graphics/CreateResource/CreateResource.h"
#include "engine/3d/Model/Animation/Animation.h"
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
	void CreateInstancingSrv(Microsoft::WRL::ComPtr<ID3D12Resource>,uint32_t index);

	/// <summary>
	/// structuredbuffer用
	/// </summary>
	/// <param name=""></param>
	/// <param name="index"></param>
	void CreateStructuredBufferSrv(Microsoft::WRL::ComPtr<ID3D12Resource> resource, uint32_t num, uint32_t size, uint32_t index);

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
	/// 空いてる番号を取り出す(texture用のsrv)
	/// </summary>
	void Allocate();

	/// <summary>
	/// 使ってないインデックス解放する(texture用のsrv)
	/// </summary>
	void Free(uint32_t index);

	/// <summary>
	/// 空いてる番号を取り出す(structuredBuf用のsrv)
	/// </summary>
	void StructuredBufIndexAllocate();

	/// <summary>
	/// 使ってないindexを解放する(structuredBuf用のsrv)
	/// </summary>
	/// <param name="index"></param>
	void StructuredBufIndexFree(uint32_t index);

	/// <summary>
	/// 空いてる番号を取り出す(uav)
	/// </summary>
	void UavAllocate();

	/// <summary>
	/// 使ってないインデックス解放する(texture用のsrv)
	/// </summary>
	void UavFree(uint32_t index);

#pragma region getter

	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle(uint32_t texHandle);
	uint32_t GetIndex() { return index_; }
	uint32_t GetStructuredBufIndex() { return structuredBufIndex_; }
	uint32_t GetUavIndex() { return uavIndex_; }

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
	static const uint32_t kMaxSRVCount = 1024 * 6;

	D3D12_CPU_DESCRIPTOR_HANDLE cpuDescHandle_[kMaxSRVCount];
	D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandle_[kMaxSRVCount];

	// texture用のsrvを作る場所を0～2047まで
	uint32_t index_ = 0; // srvのインデックス
	uint32_t beforeIndex_ = 0; // 前のインデックスを入れとく
	std::queue<uint32_t> vacantIndices_; // 空きインデックスを管理

	// structuredBuffer用のsrvを作る場所を2048～4095まで
	uint32_t structuredBufIndex_ = 0;
	uint32_t beforestructuredBufIndex_ = 2047;
	std::queue<uint32_t> vacantStructuredBufIndices_;

	// uavを作る場所を4095～6144
	uint32_t uavIndex_ = 0;
	uint32_t beforeUavIndex_ = 4095;
	std::queue<uint32_t> vacantUavIndices_;
};
