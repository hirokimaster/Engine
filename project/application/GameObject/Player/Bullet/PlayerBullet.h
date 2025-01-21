/**
* @file PlayerBullet.h
* @brief プレイヤー（自機）の弾
* @author 仁平 琉乃
*/

#pragma once
#include "engine/Utility/CollisionManager/Collider/Collider.h"
#include "application/GameObject/LockOn/LockOn.h"
#include "engine/Object3DPlacer/Object3DPlacer.h"
#include "engine/ParticleManager/ParticleManager.h"

class PlayerBullet : public Collider {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="object"></param>
	/// <param name="texHandle"></param>
	/// <param name="model"></param>
	void Initialize(uint32_t texHandle);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	void Draw(Camera& camera);

private: // クラス内でしか使わない

	/// <summary>
	/// 移動処理
	/// </summary>
	void Move();

	/// <summary>
	/// 消す
	/// </summary>
	void BulletErase();

	/// <summary>
	/// 当たり判定
	/// </summary>
	void OnCollision()override;

public:

#pragma	region getter

	bool GetIsDead() { return isDead_; }; // デスフラグ

	Vector3 GetWorldPosition()const override;

	Vector3 GetScale() const override { return worldTransform_.scale; }

#pragma endregion

#pragma region	setter

	void SetVelocity(Vector3 velocity) { velocity_ = velocity; }; // 速度ベクトル

	void SetPosition(Vector3 position) { worldTransform_.translate = position; } // 位置

	void SetLockOn(LockOn* lockOn) { lockOn_ = lockOn; }

	void SetId(uint32_t id) { id_ = id; }

#pragma endregion

private:
	Vector3 velocity_ = {};	// 移動ベクトル
	bool isDead_ = false; // bulletのデスフラグ
	static const int32_t kLifeTime_ = 60 * 5; // 生きてる時間
	int32_t deathTimer_ = kLifeTime_; // デスタイマー
	LockOn* lockOn_ = nullptr; // ロックオンのポインタ
	WorldTransform worldTransform_{};
	std::unique_ptr<Object3DPlacer> object_ = nullptr;
	ParticleManager* particleManager_ = nullptr;
	uint32_t texHandleSmoke_ = 0;
	uint32_t id_ = 0;
	int32_t particleTimer_ = 200;
};