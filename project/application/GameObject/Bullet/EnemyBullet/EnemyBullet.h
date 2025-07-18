/**
* @file EnemyBullet.h
* @brief 敵の弾
* @author 仁平 琉乃
*/

#pragma once
#include "engine/Utility/CollisionManager/Collider/Collider.h"
#include "application/GameObject/Bullet/IBullet.h"
#include <random>
#include "engine/Graphics/Effects/Particle/ParticleManager.h"
#include "engine/Math/Mathfunction.h"

class Player;

class EnemyBullet : public IBullet{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize()override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;

	/// <summary>
	/// 消滅時間のリセット
	/// </summary>
	void ResetDeathTimer()override;

private:

	/// <summary>
	/// 移動
	/// </summary>
	void Move()override;

	/// <summary>
	/// 当たり判定
	/// </summary>
	void OnCollision();

	/// <summary>
	/// 追尾処理
	/// </summary>
	void Homing();

public:

#pragma region getter

	Vector3 GetWorldPosition()const override;

	Vector3 GetScale() const { return object_.lock()->worldTransform.scale; }

	bool GetIsDead()const override { return isDead_; }

	Collider* GetCollider()override { return collider_.get(); }

	BulletType GetBulletType()override { return type_; }

	bool GetIsActive()const override { return isActive_; }

#pragma endregion

#pragma region	setter

	void SetVelocity(Vector3 velocity) { velocity_ = velocity; }; // 速度ベクトル

	void SetPosition(Vector3 position) { BaseInstancingObject::SetPosition(position); } // 位置

	void SetIsActive(bool isActive)override { isActive_ = isActive; }

	void SetIsDead(bool isDead)override { isDead_ = isDead;}

	void SetTarget(const Player* target) { target_ = target; }

#pragma endregion

private:
	Vector3 velocity_ = {};	// 速度ベクトル
	bool isDead_ = false; // bulletのデスフラグ
	static const int32_t kLifeTime_ = 60 * 5; // 生きてる時間
	int32_t deathTimer_ = kLifeTime_; // デスタイマー
	BulletType type_;
	bool isActive_ = false;
	const Player* target_ = nullptr; // ターゲット
};
