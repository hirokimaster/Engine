/**
* @file Laser.h
* @brief レーザー(障害物)
* @author 仁平 琉乃
*/


#pragma once
#include "engine/Graphics/Effects/Particle/GPUParticle.h"
#include "engine/Graphics/Effects/Particle/ParticleManager.h"
#include "engine/Graphics/Transform/WorldTransform.h"
#include "engine/Utility/CollisionManager/Collider/Collider.h"

enum class LaserType {
	Side,
	Ver
};

class Laser{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

#pragma region getter

	bool GetIsHit()const { return isHit_; }

	Vector3 GetWorldPosition()const;

	Collider* GetCollider() { return collider_.get(); }

#pragma endregion

#pragma region setter

	void SetPosition(const Vector3& position) { worldTransform_.translate = position; }

	void SetScale(const Vector3& scale) { worldTransform_.scale = scale; }

	void SetType(LaserType type) { type_ = type; }

#pragma endregion

private:

	void OnCollision();

private:
	bool isHit_ = false;
	GPUParticle* particle_ = nullptr; // particle
	ParticleManager* particleManager_ = nullptr; // particleManager	
	float lifeTime_ = 0.0f; // 生存時間
	std::unique_ptr<Collider> collider_; // collider
	WorldTransform worldTransform_; // ワールド座標
	LaserType type_; // laserのタイプ
};
