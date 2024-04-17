#pragma once
#include "engine/Sprite/Sprite.h"
#include "engine/DescriptorManager/DescriptorManager.h"

class PostProcess{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	PostProcess();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~PostProcess();

	void Initialize();

	void CreateRTV();

	void CreateDSV();

	void CreateSRV();

	void PreDraw();

	void PostDraw();

	void Draw();

	Bloom SetBloomProperty(Bloom bloom) { return *bloomData_ = bloom; }

private:
	Resource resource_{};
	Vector4* materialData_ = nullptr;
	uint32_t texHandle_ = 0;
	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff_;
	D3D12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	Microsoft::WRL::ComPtr<ID3D12Resource> depthBuffer_;
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles_;
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandles_;
	D3D12_RESOURCE_BARRIER barrier_{};
	D3D12_VIEWPORT viewport{};
	D3D12_RECT scissorRect{};
	uint32_t index_;
	Bloom* bloomData_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> bloom_;
};
