#pragma once
#include "engine/ParticleSystem/ParticleSystem.h"

class PlayerParticle {
public:

	void Initialize();

	void Update();

	void Draw(const Camera& camera);

#pragma region setter
	// setter
	void SetPosition(const Vector3& position) { position_ = position; }

	void SetVelocityXY(const Vector2& velocity) { velocity_.x = velocity.x, velocity_.y = velocity.y; }

	void SetAreaMax(const Vector3& max) { areaMax_ = max; }

	void SetAreaMin(const Vector3& min) { areaMin_ = min; }

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
	AccelerationField accelerationField{};
	Vector3 areaMin_{};
	Vector3 areaMax_{};
};