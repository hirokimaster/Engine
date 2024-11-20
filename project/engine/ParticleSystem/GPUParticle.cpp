#include "GPUParticle.h"

void GPUParticle::Initialize()
{
	// srv作成
	CreateSRV();
	// uav作成
	CreateUAV();
	// bufferResource作成
	CreateBuffer();

	commandList_ = DirectXCommon::GetInstance()->GetCommandList();
}

void GPUParticle::Draw(const Camera& camera)
{
	// perView更新
	PerViewUpdate(camera);

	graphicsPipeline_ = GraphicsPipeline::GetInstance()->GetPSO().gpuParticle;
	computePipeline_ = ComputePipeline::GetInstance()->GetPipelineType().particle;

	DirectXCommon::GetInstance()->TransitionResourceBarrier(uavResource_.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
	// dispatch
	commandList_->SetComputeRootSignature(computePipeline_.rootSignature.Get());
	commandList_->SetPipelineState(computePipeline_.computePipelineState.Get());
	// uav設定
	commandList_->SetComputeRootDescriptorTable(0, gpuDescHandle_);
	commandList_->Dispatch(1, 1, 1);
	DirectXCommon::GetInstance()->TransitionResourceBarrier(uavResource_.Get(), D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_GENERIC_READ);


	// graphics
	commandList_->SetGraphicsRootSignature(graphicsPipeline_.rootSignature_.Get());
	commandList_->SetPipelineState(graphicsPipeline_.graphicsPipelineState_.Get()); // PSOを設定
	// 形状を設定
	commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	commandList_->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	commandList_->SetGraphicsRootConstantBufferView(1, perViewResource_->GetGPUVirtualAddress());
	SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(2, srvIndex_); // structuredBuffer
	SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(3, texHandle_); // texture

	// drawCall
	if (model_){
		model_->Draw(kMaxInstance_);
    }
}

void GPUParticle::CreateUAV()
{

	Microsoft::WRL::ComPtr<ID3D12Device> device = DirectXCommon::GetInstance()->GetDevice();

	// uav作成
	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc{};
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.NumElements = 1024;
	uavDesc.Buffer.CounterOffsetInBytes = 0;
	uavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;
	uavDesc.Buffer.StructureByteStride = sizeof(ParticleCS);

	// srvと同じHeapに配置するのでsrvのマネージャからhandleの位置をずらす
	SrvManager::GetInstance()->UavAllocate();
	uavIndex_ = SrvManager::GetInstance()->GetUavIndex();

	cpuDescHandle_ = DescriptorManager::GetInstance()->GetCPUDescriptorHandle(DescriptorManager::GetInstance()->GetSRV(),
		DescriptorManager::GetInstance()->GetDescSize().SRV, uavIndex_);
	gpuDescHandle_ = DescriptorManager::GetInstance()->GetGPUDescriptorHandle(DescriptorManager::GetInstance()->GetSRV(),
		DescriptorManager::GetInstance()->GetDescSize().SRV, uavIndex_);
	device->CreateUnorderedAccessView(uavResource_.Get(), nullptr, &uavDesc, cpuDescHandle_);
	
}

void GPUParticle::CreateSRV()
{
	uavResource_ = CreateResource::CreateRWStructuredBufferResource(sizeof(ParticleCS) * kMaxInstance_);
	// srvの位置をtextureのsrvの位置から設定する
	SrvManager::GetInstance()->StructuredBufIndexAllocate();
	srvIndex_ = SrvManager::GetInstance()->GetStructuredBufIndex();
	SrvManager::GetInstance()->CreateStructuredBufferSrv(uavResource_.Get(), kMaxInstance_, sizeof(ParticleCS), srvIndex_);
}

void GPUParticle::CreateBuffer()
{
	materialResource_ = CreateResource::CreateBufferResource(sizeof(Material));
	// データを書き込む
	Material* materialData = nullptr;
	// アドレスを取得
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	materialData->color = { 1.0f,1.0f,1.0f,1.0f };

	perViewResource_ = CreateResource::CreateBufferResource(sizeof(PerView));
	perViewResource_->Map(0, nullptr, reinterpret_cast<void**>(&perViewData_));
	perViewData_->billboardMatrix = MakeIdentityMatrix();
	perViewData_->viewProjection = MakeIdentityMatrix();
}

void GPUParticle::PerViewUpdate(const Camera& camera)
{
	Matrix4x4 backToFrontMatrix = MakeRotateYMatrix(std::numbers::pi_v<float>);

	Matrix4x4 cameraMatrix = Inverse(camera.matView);

	Matrix4x4 billboardMatrix = Multiply(backToFrontMatrix, cameraMatrix);
	billboardMatrix.m[3][0] = 0.0f;
	billboardMatrix.m[3][1] = 0.0f;
	billboardMatrix.m[3][2] = 0.0f;

	perViewData_->billboardMatrix = billboardMatrix;
	perViewData_->viewProjection = Multiply(camera.matView, camera.matProjection);
}

