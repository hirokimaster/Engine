/**
* @file GPUParticle.h
* @brief GPUParticleの生成
* @author 仁平 琉乃
*/

#pragma once
#include "engine/Math/Vector3.h"
#include "engine/Math/Vector4.h"
#include "engine/Math/Matrix4x4.h"
#include "engine/Graphics/CreateResource/CreateResource.h"
#include "engine/3d/Model/ModelManager.h"
#include "engine/Graphics/Pipeline/GraphicsPipeline.h"
#include "engine/Graphics/Pipeline/ComputePipeline.h"
#include "engine/Graphics/Camera/Camera.h"

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

struct ParticleRange1d {
	float min;
	float max;
	float padding[2];
};

struct ParticleRange3d {
	Vector3 min;
	float padding1[1];
	Vector3 max;
	float padding2[1];
};

struct EmitterSphere {
	Vector3 translate;
	float padding1[1];
	Vector3 velocity;
	float radius;
	uint32_t count;
	float frequency;
	float frequencyTime;
	uint32_t emit;
	ParticleRange3d rangeTranslate;
	ParticleRange3d rangeScale;
	ParticleRange1d rangeLifeTime;
	ParticleRange3d rangeVelocity;
	ParticleRange1d rangeCurrentTime;
	ParticleRange3d rangeColor;
	ParticleRange1d rangeAlpha;
};

struct PerFrame {
	float time;
	float deltaTime;
};

class GPUParticle {
public:

	GPUParticle();
	~GPUParticle();

	void Initialize();

	void Update();

	void Draw(const Camera& camera);

	void ResetLifeTime();

#pragma region setter

	void SetModel(const std::string& fileName) { model_ = ModelManager::CreateObj(fileName); }

	void SetTexHandle(uint32_t texHandle) { texHandle_ = texHandle; }

	void SetParticleParam(EmitterSphere param) { *emitterSphereData_ = param; }

	void SetColor(const Vector4& color) { materialData_->color = color;}

	void SetPosition(const Vector3& position) { emitterSphereData_->translate = position; }

	void SetVelocity(const Vector3& velocity) { emitterSphereData_->velocity = velocity; }

	void SetIsActive(bool isActive) { isActive_ = isActive; }

	void SetIsDead(bool isDead) { isDead_ = isDead; }

	void SetLifeTime(float time) { lifeTime_ = time; }

#pragma endregion

#pragma region getter

	bool GetIsActive()const { return isActive_; }

	bool GetIsDead()const { return isDead_; }

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

	void YBillboard(const Camera& camera);

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
	EmitterSphere* emitterSphereData_;
	PerFrame* perFrameData_ = nullptr;
	Model* model_ = nullptr;
	Material* materialData_ = nullptr;
	D3D12_CPU_DESCRIPTOR_HANDLE particleCpuDescHandle_;
	D3D12_GPU_DESCRIPTOR_HANDLE particleGpuDescHandle_;
	D3D12_CPU_DESCRIPTOR_HANDLE freeListIndexCpuDescHandle_;
	D3D12_GPU_DESCRIPTOR_HANDLE freeListIndexGpuDescHandle_;
	D3D12_CPU_DESCRIPTOR_HANDLE freeListCpuDescHandle_;
	D3D12_GPU_DESCRIPTOR_HANDLE freeListGpuDescHandle_;
	uint32_t particleUavIndex_ = 0;
	uint32_t freeListUavIndex_ = 0;
	uint32_t freeListIndexUavIndex_ = 0;
	uint32_t particleSrvIndex_ = 0;
	uint32_t freeListSrvIndex_ = 0;
	uint32_t freeListIndexSrvIndex_ = 0;
	ComputePipelineData initializeParticle_;
	ComputePipelineData emitParticle_;
	ComputePipelineData updateParticle_;
	GraphicsPipelineData graphicsPipeline_;
	uint32_t texHandle_ = 0;
	D3D12_RESOURCE_BARRIER barrier_{};
	const float kDeltaTime_ = 1.0f / 60.0f;
	bool isActive_ = false;
	bool isDead_ = false;
	float lifeTime_ = 180.0f; // 生存時間
    float kLifeTime = 300.0f;
};
