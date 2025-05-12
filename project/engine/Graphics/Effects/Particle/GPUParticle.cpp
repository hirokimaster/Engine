/**
* @file GPUParticle.cpp
* @brief GPUParticleの生成
* @author 仁平 琉乃
*/

#include "GPUParticle.h"

GPUParticle::GPUParticle()
{
}

GPUParticle::~GPUParticle()
{
	// uav解放
	SrvManager::GetInstance()->UavFree(particleUavIndex_);
	SrvManager::GetInstance()->UavFree(freeListIndexUavIndex_);
	SrvManager::GetInstance()->UavFree(freeListUavIndex_);

	// srv解放
	SrvManager::GetInstance()->StructuredBufIndexFree(particleSrvIndex_);
	SrvManager::GetInstance()->StructuredBufIndexFree(freeListIndexSrvIndex_);
	SrvManager::GetInstance()->StructuredBufIndexFree(freeListSrvIndex_);
}

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

	isActive_ = false;
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
	SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(2, particleSrvIndex_); // structuredBuffer
	SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(3, texHandle_); // texture

	// drawCall
	if (model_) {
		model_->Draw(kMaxInstance_);
	}
}

void GPUParticle::ResetLifeTime()
{
	lifeTime_ = kLifeTime;
}

void GPUParticle::CreateUAV()
{

	Microsoft::WRL::ComPtr<ID3D12Device> device = DirectXCommon::GetInstance()->GetDevice();

	// particleUAV作成
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
	particleUavIndex_ = SrvManager::GetInstance()->GetUavIndex();

	particleCpuDescHandle_ = DescriptorManager::GetInstance()->GetCPUDescriptorHandle(DescriptorManager::GetInstance()->GetSRV(),
		DescriptorManager::GetInstance()->GetDescSize().SRV, particleUavIndex_);
	particleGpuDescHandle_ = DescriptorManager::GetInstance()->GetGPUDescriptorHandle(DescriptorManager::GetInstance()->GetSRV(),
		DescriptorManager::GetInstance()->GetDescSize().SRV, particleUavIndex_);
	device->CreateUnorderedAccessView(particleResource_.Get(), nullptr, &uavDesc, particleCpuDescHandle_);

	// freeListIndex
	D3D12_UNORDERED_ACCESS_VIEW_DESC freeListIndexUavDesc{};
	freeListIndexUavDesc.Format = DXGI_FORMAT_UNKNOWN;
	freeListIndexUavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	freeListIndexUavDesc.Buffer.FirstElement = 0;
	freeListIndexUavDesc.Buffer.NumElements = 1;
	freeListIndexUavDesc.Buffer.CounterOffsetInBytes = 0;
	freeListIndexUavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;
	freeListIndexUavDesc.Buffer.StructureByteStride = sizeof(int32_t);
	// srvと同じHeapに配置するのでsrvのマネージャからhandleの位置をずらす
	SrvManager::GetInstance()->UavAllocate();
	freeListIndexUavIndex_ = SrvManager::GetInstance()->GetUavIndex();

	freeListIndexCpuDescHandle_ = DescriptorManager::GetInstance()->GetCPUDescriptorHandle(DescriptorManager::GetInstance()->GetSRV(),
		DescriptorManager::GetInstance()->GetDescSize().SRV, freeListIndexUavIndex_);
	freeListIndexGpuDescHandle_ = DescriptorManager::GetInstance()->GetGPUDescriptorHandle(DescriptorManager::GetInstance()->GetSRV(),
		DescriptorManager::GetInstance()->GetDescSize().SRV, freeListIndexUavIndex_);
	device->CreateUnorderedAccessView(freeListIndexResource_.Get(), nullptr, &freeListIndexUavDesc, freeListIndexCpuDescHandle_);

	// freeList
	D3D12_UNORDERED_ACCESS_VIEW_DESC freeListUavDesc{};
	freeListUavDesc.Format = DXGI_FORMAT_UNKNOWN;
	freeListUavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	freeListUavDesc.Buffer.FirstElement = 0;
	freeListUavDesc.Buffer.NumElements = kMaxInstance_;
	freeListUavDesc.Buffer.CounterOffsetInBytes = 0;
	freeListUavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;
	freeListUavDesc.Buffer.StructureByteStride = sizeof(uint32_t);
	// srvと同じHeapに配置するのでsrvのマネージャからhandleの位置をずらす
	SrvManager::GetInstance()->UavAllocate();
	freeListUavIndex_ = SrvManager::GetInstance()->GetUavIndex();

	freeListCpuDescHandle_ = DescriptorManager::GetInstance()->GetCPUDescriptorHandle(DescriptorManager::GetInstance()->GetSRV(),
		DescriptorManager::GetInstance()->GetDescSize().SRV, freeListUavIndex_);
	freeListGpuDescHandle_ = DescriptorManager::GetInstance()->GetGPUDescriptorHandle(DescriptorManager::GetInstance()->GetSRV(),
		DescriptorManager::GetInstance()->GetDescSize().SRV, freeListUavIndex_);
	device->CreateUnorderedAccessView(freeListResource_.Get(), nullptr, &freeListUavDesc, freeListCpuDescHandle_);

}

void GPUParticle::CreateSRV()
{
	// particle
	particleResource_ = CreateResource::CreateRWStructuredBufferResource(sizeof(ParticleCS) * kMaxInstance_);
	SrvManager::GetInstance()->StructuredBufIndexAllocate();
	particleSrvIndex_ = SrvManager::GetInstance()->GetStructuredBufIndex();
	SrvManager::GetInstance()->CreateStructuredBufferSrv(particleResource_.Get(), kMaxInstance_, sizeof(ParticleCS), particleSrvIndex_);

	// freeListIndex
	freeListIndexResource_ = CreateResource::CreateRWStructuredBufferResource(sizeof(int32_t));
	SrvManager::GetInstance()->StructuredBufIndexAllocate();
	freeListIndexSrvIndex_ = SrvManager::GetInstance()->GetStructuredBufIndex();
	SrvManager::GetInstance()->CreateStructuredBufferSrv(freeListIndexResource_.Get(), 1, sizeof(int32_t), freeListIndexSrvIndex_);

	// freeList
	freeListResource_ = CreateResource::CreateRWStructuredBufferResource(sizeof(uint32_t) * kMaxInstance_);
	SrvManager::GetInstance()->StructuredBufIndexAllocate();
	freeListSrvIndex_ = SrvManager::GetInstance()->GetStructuredBufIndex();
	SrvManager::GetInstance()->CreateStructuredBufferSrv(freeListResource_.Get(), kMaxInstance_, sizeof(int32_t), freeListSrvIndex_);
}

void GPUParticle::CreateBuffer()
{
	materialResource_ = CreateResource::CreateBufferResource(sizeof(Material));
	// データを書き込む
	// アドレスを取得
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	materialData_->color = { 1.0f,1.0f,1.0f,1.0f };
	materialData_->enableLighting = false;

	perViewResource_ = CreateResource::CreateBufferResource(sizeof(PerView));
	perViewResource_->Map(0, nullptr, reinterpret_cast<void**>(&perViewData_));
	perViewData_->billboardMatrix = MakeIdentityMatrix();
	perViewData_->viewProjection = MakeIdentityMatrix();

	// emitter
	emitterSphereResource_ = CreateResource::CreateBufferResource(sizeof(EmitterSphere));
	emitterSphereResource_->Map(0, nullptr, reinterpret_cast<void**>(&emitterSphereData_));
	
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

void GPUParticle::YBillboard(const Camera& camera)
{
	// カメラのワールド行列を取得
	Matrix4x4 cameraMatrix = Inverse(camera.matView);

	// カメラの前方・右方向ベクトルを取り出す
	Vector3 cameraForward = {
		cameraMatrix.m[2][0],
		0.0f, // Y軸成分を無視（平面に投影）
		cameraMatrix.m[2][2]
	};
	cameraForward = Normalize(cameraForward);

	// カメラの右ベクトルを再計算（Y軸ビルボードなのでY軸は固定）
	Vector3 cameraRight = Cross({ 0.0f, 1.0f, 0.0f }, cameraForward);
	cameraRight = Normalize(cameraRight);

	// カメラの上方向（Y軸）固定
	Vector3 cameraUp = { 0.0f, 1.0f, 0.0f };

	Matrix4x4 billboardMatrix = {
		cameraRight.x, cameraRight.y, cameraRight.z, 0.0f,
		cameraUp.x,    cameraUp.y,    cameraUp.z,    0.0f,
		cameraForward.x, cameraForward.y, cameraForward.z, 0.0f,
		0.0f,          0.0f,          0.0f,          1.0f
	};

	// 180度回転（後ろ向き対策）
	Matrix4x4 backToFrontMatrix = MakeRotateYMatrix(std::numbers::pi_v<float>);
	billboardMatrix = Multiply(backToFrontMatrix, billboardMatrix);

	// 適用
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

	if (--lifeTime_ <= 0.0f) {
	    isDead_ = true;
	}
}

void GPUParticle::InitializeParticleCS()
{
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeaps[] = { DescriptorManager::GetInstance()->GetSRV() };
	commandList_->SetDescriptorHeaps(1, descriptorHeaps->GetAddressOf());

	initializeParticle_ = ComputePipeline::GetInstance()->GetPipelineType().initializeParticle;

	DirectXCommon::GetInstance()->TransitionResourceBarrier(particleResource_.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
	DirectXCommon::GetInstance()->TransitionResourceBarrier(freeListIndexResource_.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
	DirectXCommon::GetInstance()->TransitionResourceBarrier(freeListResource_.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
	// dispatch
	commandList_->SetComputeRootSignature(initializeParticle_.rootSignature.Get());
	commandList_->SetPipelineState(initializeParticle_.computePipelineState.Get());
	// uav設定
	commandList_->SetComputeRootDescriptorTable(0, particleGpuDescHandle_);
	commandList_->SetComputeRootDescriptorTable(1, freeListIndexGpuDescHandle_);
	commandList_->SetComputeRootDescriptorTable(2, freeListGpuDescHandle_);
	commandList_->Dispatch(1, 1, 1);
}

void GPUParticle::EmitterParticleCS()
{
	emitParticle_ = ComputePipeline::GetInstance()->GetPipelineType().emitterParticle;

	commandList_->SetComputeRootSignature(emitParticle_.rootSignature.Get());
	commandList_->SetPipelineState(emitParticle_.computePipelineState.Get());
	commandList_->SetComputeRootDescriptorTable(0, particleGpuDescHandle_);
	commandList_->SetComputeRootConstantBufferView(1, emitterSphereResource_->GetGPUVirtualAddress());
	commandList_->SetComputeRootConstantBufferView(2, perFrameResource_->GetGPUVirtualAddress());
	commandList_->SetComputeRootDescriptorTable(3, freeListIndexGpuDescHandle_);
	commandList_->SetComputeRootDescriptorTable(4, freeListGpuDescHandle_);
	commandList_->Dispatch(1, 1, 1);
}

void GPUParticle::UpdateParticleCS()
{
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.UAV.pResource = particleResource_.Get();
	commandList_->ResourceBarrier(1, &barrier);

	updateParticle_ = ComputePipeline::GetInstance()->GetPipelineType().updateParticle;

	commandList_->SetComputeRootSignature(updateParticle_.rootSignature.Get());
	commandList_->SetPipelineState(updateParticle_.computePipelineState.Get());
	commandList_->SetComputeRootDescriptorTable(0, particleGpuDescHandle_);
	commandList_->SetComputeRootConstantBufferView(1, perFrameResource_->GetGPUVirtualAddress());
	commandList_->SetComputeRootDescriptorTable(2, freeListIndexGpuDescHandle_);
	commandList_->SetComputeRootDescriptorTable(3, freeListGpuDescHandle_);
	commandList_->Dispatch(1, 1, 1);
	DirectXCommon::GetInstance()->TransitionResourceBarrier(particleResource_.Get(), D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_GENERIC_READ);
	DirectXCommon::GetInstance()->TransitionResourceBarrier(freeListIndexResource_.Get(), D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_GENERIC_READ);
	DirectXCommon::GetInstance()->TransitionResourceBarrier(freeListResource_.Get(), D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_GENERIC_READ);
}

