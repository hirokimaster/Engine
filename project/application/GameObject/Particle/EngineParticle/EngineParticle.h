/**
* @file EngineParticle.h
* @brief 自機のエンジンのパーティクル
* @author 仁平 琉乃
*/

#pragma once
#include "engine/Graphics/Effects/Particle/ParticleManager.h"

class EngineParticle {
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="playerRotate"></param>
	/// <param name="playerPosition"></param>
	void Update(const Vector3& playerRotate, const Vector3& playerPosition);

#pragma region setter

	void SetLifeTime(float time) { lifeTime_ = time; }

	void SetIsActive(bool flag) {
		leftEngine_->SetIsActive(false);
		rightEngine_->SetIsActive(false);
	}

#pragma endregion

private:

	ParticleManager* particleManager_ = nullptr;
	GPUParticle* rightEngine_ = nullptr;
	GPUParticle* leftEngine_ = nullptr;
	Vector3 particleOffsetL_{ -0.3f,0.0f,-3.8f };
	Vector3 particleOffsetR_{ 0.3f,0.0f,-3.8f };
	float lifeTime_ = 0.0f;
};
