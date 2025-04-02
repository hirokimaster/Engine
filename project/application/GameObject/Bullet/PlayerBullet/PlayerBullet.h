/**
* @file PlayerBullet.h
* @brief プレイヤー（自機）の弾
* @author 仁平 琉乃
*/

#pragma once
#include "engine/Utility/CollisionManager/Collider/Collider.h"
#include "application/GameObject/LockOn/LockOn.h"
#include "application/GameObject/Bullet/IBullet.h"
#include "engine/Graphics/Effects/Particle/ParticleManager.h"

class PlayerBullet : public IBullet{
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
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	void Draw(const Camera& camera)override;

	/// <summary>
	/// 消滅時間のリセット
	/// </summary>
	void ResetDeathTimer()override;

private: // クラス内でしか使わない

	/// <summary>
	/// 移動処理
	/// </summary>
	void Move()override;

	/// <summary>
	/// 当たり判定
	/// </summary>
	void OnCollision();

public:

#pragma	region getter

	bool GetIsDead()const override { return isDead_; }; // デスフラグ

	Vector3 GetWorldPosition()const override;

	Vector3 GetScale() const { return object_->GetWorldTransform().scale; }

	Collider* GetCollider()override { return collider_.get(); }

	BulletType GetBulletType()override { return type_; }

	bool GetIsActive()const override { return isActive_; }

#pragma endregion

#pragma region	setter

	void SetVelocity(const Vector3& velocity) { velocity_ = velocity; }; // 速度ベクトル

	void SetPosition(const Vector3& position) { object_->SetPosition(position); } // 位置

	void SetLockOn(LockOn* lockOn) { lockOn_ = lockOn; }

	void SetIsActive(bool isActive)override { isActive_ = isActive; }

	void SetIsDead(bool isDead)override { isDead_ = isDead; }

#pragma endregion

private:
	Vector3 velocity_ = {};	// 移動ベクトル
	bool isDead_ = false; // bulletのデスフラグ
	static const int32_t kLifeTime_ = 60 * 3; // 生きてる時間
	int32_t deathTimer_ = kLifeTime_; // デスタイマー
	LockOn* lockOn_ = nullptr; // ロックオンのポインタ
	BulletType type_;
	bool isActive_ = false;
};