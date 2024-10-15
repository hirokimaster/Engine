#include "DescriptorManager.h"

DescriptorManager* DescriptorManager::GetInstance()
{
	static DescriptorManager instance;
	return &instance;
}

void DescriptorManager::Initialize()
{
	srvHeap_ = CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 1024, true);
	rtvHeap_ = CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 3, false);
	dsvHeap_ = CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1, false);
	size_.SRV = DirectXCommon::GetInstance()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	size_.RTV = DirectXCommon::GetInstance()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	size_.DSV = DirectXCommon::GetInstance()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
}

Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> DescriptorManager::CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE type, UINT numDescriptors, bool shaderVisible)
{
	// ディスクリプタヒープの生成
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> DescriptorHeap = nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC DescriptorHeapDesc{};
	DescriptorHeapDesc.Type = type;
	DescriptorHeapDesc.NumDescriptors = numDescriptors; // ダブルバッファ用に2つ。多くても別に構わない
	DescriptorHeapDesc.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	hr_ = DirectXCommon::GetInstance()->GetDevice()->CreateDescriptorHeap(&DescriptorHeapDesc, IID_PPV_ARGS(&DescriptorHeap));
	// ディスクリプタヒープが作れなっかたので起動できない
	assert(SUCCEEDED(hr_));

	return DescriptorHeap;
}

D3D12_CPU_DESCRIPTOR_HANDLE DescriptorManager::GetCPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index)
{
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (descriptorSize * index);
	return handleCPU;
}

D3D12_GPU_DESCRIPTOR_HANDLE DescriptorManager::GetGPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index)
{
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (descriptorSize * index);
	return handleGPU;
}

