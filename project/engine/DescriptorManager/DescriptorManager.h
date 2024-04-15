#pragma once
#include "engine/Base/DX/DirectXCommon.h"
#include <DirectXTex.h>

struct descSize {
	//size
	uint32_t SRV = 0;
	uint32_t RTV = 0;
	uint32_t DSV = 0;
};

class DescriptorManager {
public:

	/// <summary>
	/// シングルトンインスタンス
	/// </summary>
	/// <returns></returns>
	static DescriptorManager* GetInstance();

	void Initialize();

	// descriptorheap生成
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE type, UINT numDescriptors, bool shaderVisible);

#pragma region Getter & Setter

	ID3D12DescriptorHeap* GetSRV() { return srvHeap_.Get(); }
	ID3D12DescriptorHeap* GetRTV() { return rtvHeap_.Get(); }
	ID3D12DescriptorHeap* GetDSV() { return dsvHeap_.Get(); }
	static descSize GetDescSize() { return DescriptorManager::GetInstance()->size_; }
	static D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index);
	static D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index);

#pragma endregion

private:
	DescriptorManager() = default;
	~DescriptorManager() = default;
	DescriptorManager(const DescriptorManager&) = delete;
	DescriptorManager& operator=(const DescriptorManager&) = delete;

private:
	HRESULT hr_;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap_;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvHeap_;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvHeap_;
	descSize size_ = {};
};
