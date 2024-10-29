#pragma once
#include "engine/ParticleSystem/ParticleSystem.h"

class ExplosionParticle {
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	void Draw(const Camera& camera);

#pragma region setter
	// setter
	void SetPosition(const Vector3& position) { position_ = position; }

	void SetVelocityXY(const Vector2& velocity) { velocity_.x = velocity.x, velocity_.y = velocity.y; }

#pragma endregion

private:
	std::unique_ptr<ParticleSystem> particle_ = {};
	std::list<Particle> particles_ = {};
	const float deltaTime_ = 1.0f / 60.0f;
	Emitter emit_{};
	std::mt19937 randomEngine_;
	uint32_t texHandle_ = 0;
	Vector3 position_ = {};
	Vector3 velocity_{};
	DistributionParam distributionParam_{};

};
