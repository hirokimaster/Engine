#pragma once
#include "engine/Utility/CollisionManager/Collider/Collider.h"
#include "engine/Object3DPlacer/Object3DPlacer.h"
#include <random>

class EnemyBullet : public Collider {
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

private:

	/// <summary>
	/// 移動
	/// </summary>
	void Move();

	/// <summary>
	/// 弾の削除
	/// </summary>
	void BulletErase();

	/// <summary>
	/// 当たり判定
	/// </summary>
	void OnCollision()override;

	Matrix4x4 Rotation(float angle, const Vector3& axis);

public:

#pragma region getter

	Vector3 GetWorldPosition()const override;

	bool GetIsDead() { return isDead_; }

#pragma endregion

#pragma region	setter

	void SetVelocity(Vector3 velocity) { velocity_ = velocity; }; // 速度ベクトル

	void SetPosition(Vector3 position) { worldTransform_.translate = position; } // 位置

	void SetRotateAngle(float rotate) {rotateAngle_  = rotate; } // 回転

	void SetTarget(const Vector3& target) { target_ = target; }

#pragma endregion

private:
	Vector3 velocity_ = {};	// 速度ベクトル
	bool isDead_ = false; // bulletのデスフラグ
	static const int32_t kLifeTime_ = 60 * 5; // 生きてる時間
	int32_t deathTimer_ = kLifeTime_; // デスタイマー
	WorldTransform worldTransform_{};
	std::unique_ptr<Object3DPlacer> object_ = nullptr;
	float rotateAngle_ = 0.0f;
	Vector3 target_{};
};
