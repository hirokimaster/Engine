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

	InitializeParticleCS();
}

void GPUParticle::Update()
{
	UpdateEmitter();
}

void GPUParticle::Draw(const Camera& camera)
{
	// perView更新
	PerViewUpdate(camera);

	// emitterのCS起動
	EmitterParticleCS();
	// updateParticleのCS起動
	UpdateParticleCS();

	graphicsPipeline_ = GraphicsPipeline::GetInstance()->GetPSO().gpuParticle;
	
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
	uavDesc.Buffer.NumElements = kMaxInstance_;
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

	// freeCounter
	D3D12_UNORDERED_ACCESS_VIEW_DESC freeCounterUavDesc{};
	freeCounterUavDesc.Format = DXGI_FORMAT_UNKNOWN;
	freeCounterUavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	freeCounterUavDesc.Buffer.FirstElement = 0;
	freeCounterUavDesc.Buffer.NumElements = 1;
	freeCounterUavDesc.Buffer.CounterOffsetInBytes = 0;
	freeCounterUavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;
	freeCounterUavDesc.Buffer.StructureByteStride = sizeof(int32_t);
	// srvと同じHeapに配置するのでsrvのマネージャからhandleの位置をずらす
	SrvManager::GetInstance()->UavAllocate();
	uavIndex_ = SrvManager::GetInstance()->GetUavIndex();

	freeCounterCpuDescHandle_ = DescriptorManager::GetInstance()->GetCPUDescriptorHandle(DescriptorManager::GetInstance()->GetSRV(),
		DescriptorManager::GetInstance()->GetDescSize().SRV, uavIndex_);
	freeCounterGpuDescHandle_ = DescriptorManager::GetInstance()->GetGPUDescriptorHandle(DescriptorManager::GetInstance()->GetSRV(),
		DescriptorManager::GetInstance()->GetDescSize().SRV, uavIndex_);
	device->CreateUnorderedAccessView(freeCounterResource_.Get(), nullptr, &freeCounterUavDesc, freeCounterCpuDescHandle_);
	
}

void GPUParticle::CreateSRV()
{
	// particle
	uavResource_ = CreateResource::CreateRWStructuredBufferResource(sizeof(ParticleCS) * kMaxInstance_);
	// srvの位置をtextureのsrvの位置から設定する
	SrvManager::GetInstance()->StructuredBufIndexAllocate();
	srvIndex_ = SrvManager::GetInstance()->GetStructuredBufIndex();
	SrvManager::GetInstance()->CreateStructuredBufferSrv(uavResource_.Get(), kMaxInstance_, sizeof(ParticleCS), srvIndex_);

	// freeCounter
	// srvの位置をtextureのsrvの位置から設定する
	freeCounterResource_ = CreateResource::CreateRWStructuredBufferResource(sizeof(int32_t));
	SrvManager::GetInstance()->StructuredBufIndexAllocate();
	SrvManager::GetInstance()->CreateStructuredBufferSrv(freeCounterResource_.Get(), 1, sizeof(int32_t), SrvManager::GetInstance()->GetStructuredBufIndex());
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

	// emitter
	emitterSphereResource_ = CreateResource::CreateBufferResource(sizeof(EmitterSphere));
	emitterSphereResource_->Map(0, nullptr, reinterpret_cast<void**>(&emitterSphereData_));
	emitterSphereData_->count = 10;
	emitterSphereData_->frequency = 0.5f;
	emitterSphereData_->frequencyTime = 0.0f;
	emitterSphereData_->translate = Vector3(0.0f, 0.0f, 0.0f);
	emitterSphereData_->radius = 1.0f;
	emitterSphereData_->emit = 0;

	// perFrame
	perFrameResource_ = CreateResource::CreateBufferResource(sizeof(PerFrame));
	perFrameResource_->Map(0, nullptr, reinterpret_cast<void**>(&perFrameData_));
	perFrameData_->time = 0.0f;
	perFrameData_->deltaTime = kDeltaTime_;
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

void GPUParticle::UpdateEmitter()
{
	perFrameData_->time += perFrameData_->deltaTime;
	emitterSphereData_->frequencyTime += kDeltaTime_;
	// 射出間隔を上回ったら許可を出して時間を調整する
	if (emitterSphereData_->frequency <= emitterSphereData_->frequencyTime) {
		emitterSphereData_->frequencyTime -= emitterSphereData_->frequency;
		emitterSphereData_->emit = 1;
	}
	else {
		emitterSphereData_->emit = 0;
	}
}

void GPUParticle::InitializeParticleCS()
{
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeaps[] = { DescriptorManager::GetInstance()->GetSRV() };
	commandList_->SetDescriptorHeaps(1, descriptorHeaps->GetAddressOf());

	initializeParticle_ = ComputePipeline::GetInstance()->GetPipelineType().initializeParticle;

	DirectXCommon::GetInstance()->TransitionResourceBarrier(uavResource_.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
	DirectXCommon::GetInstance()->TransitionResourceBarrier(freeCounterResource_.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
	// dispatch
	commandList_->SetComputeRootSignature(initializeParticle_.rootSignature.Get());
	commandList_->SetPipelineState(initializeParticle_.computePipelineState.Get());
	// uav設定
	commandList_->SetComputeRootDescriptorTable(0, gpuDescHandle_);
	commandList_->SetComputeRootDescriptorTable(1, freeCounterGpuDescHandle_);
	commandList_->Dispatch(1, 1, 1);


}

void GPUParticle::EmitterParticleCS()
{
	emitParticle_ = ComputePipeline::GetInstance()->GetPipelineType().emitterParticle;

	commandList_->SetComputeRootSignature(emitParticle_.rootSignature.Get());
	commandList_->SetPipelineState(emitParticle_.computePipelineState.Get());
	commandList_->SetComputeRootDescriptorTable(0, gpuDescHandle_);
	commandList_->SetComputeRootConstantBufferView(1, emitterSphereResource_->GetGPUVirtualAddress());
	commandList_->SetComputeRootConstantBufferView(2, perFrameResource_->GetGPUVirtualAddress());
	commandList_->SetComputeRootDescriptorTable(3, freeCounterGpuDescHandle_);
	commandList_->Dispatch(1, 1, 1);
}

void GPUParticle::UpdateParticleCS()
{
	updateParticle_ = ComputePipeline::GetInstance()->GetPipelineType().updateParticle;

	commandList_->SetComputeRootSignature(updateParticle_.rootSignature.Get());
	commandList_->SetPipelineState(updateParticle_.computePipelineState.Get());
	commandList_->SetComputeRootDescriptorTable(0, gpuDescHandle_);
	commandList_->SetComputeRootConstantBufferView(1, perFrameResource_->GetGPUVirtualAddress());
	commandList_->Dispatch(1, 1, 1);
	DirectXCommon::GetInstance()->TransitionResourceBarrier(uavResource_.Get(), D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_GENERIC_READ);
	DirectXCommon::GetInstance()->TransitionResourceBarrier(freeCounterResource_.Get(), D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_GENERIC_READ);
}

