#pragma once
#include "engine/Math/Vector3.h"
#include "engine/Math/Vector4.h"
#include "engine/Math/Matrix4x4.h"
#include "engine/CreateResource/CreateResource.h"
#include "engine/ModelManager/ModelManager.h"
#include "engine/GraphicsPipeline/GraphicsPipeline.h"
#include "engine/ComputePipeline/ComputePipeline.h"

static const uint32_t kMaxInstance_ = 1024;

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

struct EmitterSphere {
	Vector3 translate;
	float radius;
	uint32_t count;
	float frequency;
	float frequencyTime;
	uint32_t emit;
};

struct PerFrame {
	float time;
	float deltaTime;
};

class GPUParticle {
public:

	void Initialize();

	void Update();

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

	/// <summary>
	/// emitterの更新
	/// </summary>
	void UpdateEmitter();

	/// <summary>
	/// initialzeParticle.CSのディスパッチまで
	/// </summary>
	void InitializeParticleCS();

	/// <summary>
	/// emitterParticle.CSのディスパッチまで
	/// </summary>
	void EmitterParticleCS();

	/// <summary>
	/// updateParticle.CSのディスパッチまで
	/// </summary>
	void UpdateParticleCS();

private:
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList_;
	// uav用のリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> particleResource_;
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_;
	Microsoft::WRL::ComPtr<ID3D12Resource> perViewResource_;
	Microsoft::WRL::ComPtr<ID3D12Resource> emitterSphereResource_;
	Microsoft::WRL::ComPtr<ID3D12Resource> perFrameResource_;
	Microsoft::WRL::ComPtr<ID3D12Resource> freeListIndexResource_;
	Microsoft::WRL::ComPtr<ID3D12Resource> freeListResource_;
	ParticleCS* particleData_ = nullptr;
	PerView* perViewData_ = nullptr;
	EmitterSphere* emitterSphereData_ = nullptr;
	PerFrame* perFrameData_ = nullptr;
	Model* model_ = nullptr;
	D3D12_CPU_DESCRIPTOR_HANDLE particleCpuDescHandle_;
	D3D12_GPU_DESCRIPTOR_HANDLE particleGpuDescHandle_;
	D3D12_CPU_DESCRIPTOR_HANDLE freeListIndexCpuDescHandle_;
	D3D12_GPU_DESCRIPTOR_HANDLE freeListIndexGpuDescHandle_;
	D3D12_CPU_DESCRIPTOR_HANDLE freeListCpuDescHandle_;
	D3D12_GPU_DESCRIPTOR_HANDLE freeListGpuDescHandle_;
	uint32_t uavIndex_ = 0;
	uint32_t particleSrvIndex_ = 0;
	ComputePipelineData initializeParticle_;
	ComputePipelineData emitParticle_;
	ComputePipelineData updateParticle_;
	GraphicsPipelineData graphicsPipeline_;
	uint32_t texHandle_ = 0;
	D3D12_RESOURCE_BARRIER barrier_{};
	const float kDeltaTime_ = 1.0f / 60.0f;
};
