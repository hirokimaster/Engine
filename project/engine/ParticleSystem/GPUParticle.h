#pragma once
#include "engine/Math/Vector3.h"
#include "engine/Math/Vector4.h"
#include "engine/Math/Matrix4x4.h"
#include "engine/CreateResource/CreateResource.h"
#include "engine/ModelManager/ModelManager.h"
#include "engine/GraphicsPipeline/GraphicsPipeline.h"
#include "engine/ComputePipeline/ComputePipeline.h"

struct ParticleCS {
	Vector3 translate;
	Vector3 scale;
	float lifeTime;
	Vector3 velocity;
	float currentTime;
	Vector4 color;
};

struct PerView {
	Matrix4x4 viewProjection;
	Matrix4x4 billboardMatrix;
};

class GPUParticle {
public:

	void Initialize();

	void Draw(const Camera& camera);

#pragma region setter

	void SetModel(const std::string& fileName) { model_ = ModelManager::CreateObj(fileName); }

	void SetTexHandle(uint32_t texHandle) { texHandle_ = texHandle; }

#pragma endregion

private:
	/// <summary>
	/// UAV作成
	/// </summary>
	void CreateUAV();

	/// <summary>
	/// srv作成
	/// </summary>
	void CreateSRV();

	/// <summary>
	/// buffer作成
	/// </summary>
	void CreateBuffer();

	/// <summary>
	/// perViewに値を入れる
	/// </summary>
	void PerViewUpdate(const Camera& camera);

private:
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList_;
	// uav用のリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> uavResource_;
	Microsoft::WRL::ComPtr<ID3D12Resource> srvResource_;
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_;
	Microsoft::WRL::ComPtr<ID3D12Resource> perViewResource_;
	ParticleCS* particleData_ = nullptr;
	PerView* perViewData_ = nullptr;
	Model* model_ = nullptr;
	D3D12_CPU_DESCRIPTOR_HANDLE cpuDescHandle_;
	D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandle_;
	uint32_t uavIndex_ = 0;
	uint32_t srvIndex_ = 0;
	ComputePipelineData computePipeline_;
	GraphicsPipelineData graphicsPipeline_;
	uint32_t texHandle_ = 0;
};
