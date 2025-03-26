/**
* @file EnemyBullet.h
* @brief 敵の弾
* @author 仁平 琉乃
*/

#pragma once
#include "engine/Utility/CollisionManager/Collider/Collider.h"
#include "application/GameObject/Bullet/IBullet.h"
#include <random>

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
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	void Draw(const Camera& camera)override;

private:

	/// <summary>
	/// 移動
	/// </summary>
	void Move()override;

	/// <summary>
	/// 弾の削除
	/// </summary>
	void BulletErase();

	/// <summary>
	/// 当たり判定
	/// </summary>
	void OnCollision();

public:

#pragma region getter

	Vector3 GetWorldPosition()const override;

	Vector3 GetScale() const{ return object_->GetWorldTransform().scale; }

	bool GetIsDead()const override { return isDead_; }

	Collider* GetCollider()override { return collider_.get(); }

#pragma endregion

#pragma region	setter

	void SetVelocity(Vector3 velocity) { velocity_ = velocity; }; // 速度ベクトル

	void SetPosition(Vector3 position) { object_->SetPosition(position); } // 位置
#pragma endregion

private:
	Vector3 velocity_ = {};	// 速度ベクトル
	bool isDead_ = false; // bulletのデスフラグ
	static const int32_t kLifeTime_ = 60 * 5; // 生きてる時間
	int32_t deathTimer_ = kLifeTime_; // デスタイマー
};
