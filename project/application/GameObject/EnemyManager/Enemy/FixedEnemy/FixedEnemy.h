/**
* @file FixedEnemy.h
* @brief 動かない敵
* @author 仁平 琉乃
*/

#pragma once
#include "application/GameObject/EnemyManager/Enemy/IEnemy.h"
#include "application/GameObject/Bullet/BulletObjectPool/BulletObjectPool.h"
#include "engine/3d/PlaneProjectionShadow/PlaneProjectionShadow.h"

class FixedEnemy : public IEnemy {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize()override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;

#pragma region getter

	Vector3 GetWorldPosition()const override;

	bool GetIsDead()const override { return isDead_; }

	Collider* GetCollider()override { return collider_.get(); }

#pragma endregion

#pragma region setter

	void SetPlayer(Player* player) { player_ = player; }

	void SetBulletObjectPool(BulletObjectPool* ptr) { bulletObjectPool_ = ptr; }

	void SetPosition(const Vector3& position)override { BaseInstancingObject::SetPosition(position); }

	void SetScale(const Vector3& scale) { BaseInstancingObject::SetScale(scale); }

#pragma endregion

private:

	/// <summary>
	/// 攻撃
	/// </summary>
	void Fire()override;

	/// <summary>
	/// 当たり判定
	/// </summary>
	void OnCollision();

private:

	// デスフラグ
	bool isDead_;
	// playerのポインタ
	Player* player_ = nullptr;
	// 弾のスピード
	float bulletSpeed_ = 0.0f;
	// 弾のオブジェクトプール
	BulletObjectPool* bulletObjectPool_ = nullptr;
	// パーティクル
	ParticleManager* particleManager_ = nullptr;
	GPUParticle* particle_ = nullptr;
	bool isHit_ = false; // 当たったか
	bool isExploded_ = false; // 爆発してるか
	// 攻撃間隔
	const uint32_t kFireInterval_ = 120;
	uint32_t fireTimer_ = 120; // 攻撃タイマー
	std::unique_ptr<PlaneProjectionShadow<InstanceWorldTransform>> shadow_; // 影
};
