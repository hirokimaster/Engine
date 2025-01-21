#pragma once
#include "engine/Math/Mathfunction.h"
#include "engine/CreateResource/CreateResource.h"
#include "engine/ParticleManager/ParticleManager.h"

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

class ParticleEmitter {
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ParticleEmitter();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ParticleEmitter();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();


	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// particle発生
	/// </summary>
	void Emit();

private:
	/// <summary>
	/// buffer作成
	/// </summary>
	void CreateBuffer();

	/// <summary>
	/// リソースをUnmapする
	/// </summary>
	void Unmap();

public:

#pragma region setter

	void SetParam(const EmitterSphere& param) { *emitterSphereData_ = param; }

	void SetPosition(const Vector3& position) { emitterSphereData_->translate = position; }

#pragma endregion

#pragma region getter

	Microsoft::WRL::ComPtr<ID3D12Resource> GetResource() { return emitterSphereResource_; }

	EmitterSphere* GetEmitterData() { return emitterSphereData_; }

#pragma endregion

private:
	Microsoft::WRL::ComPtr<ID3D12Resource> emitterSphereResource_;
	EmitterSphere* emitterSphereData_ = nullptr;
	ParticleManager* particleManager_ = nullptr;
};
