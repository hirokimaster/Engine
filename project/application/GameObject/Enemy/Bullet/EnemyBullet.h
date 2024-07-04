#pragma once
#include "application/GameObject/BaseObject/BaseObject.h"
#include "engine/Utility/CollisionManager/Collider/Collider.h"

class EnemyBullet : public BaseObject, public Collider{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="object"></param>
	/// <param name="texHandle"></param>
	/// <param name="model"></param>
	void Initialize(Object3DPlacer* object, uint32_t texHandle, const std::string& model)override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	void Draw(Camera& camera)override;

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

public:

#pragma region getter

	Vector3 GetWorldPosition()override;

	bool GetIsDead() { return isDead_; }

#pragma endregion

#pragma region	setter

	void SetVelocity(Vector3 velocity) { velocity_ = velocity; }; // 速度ベクトル

	void SetPosition(Vector3 position) { worldTransform_.translate = position; } // 位置

#pragma endregion

private:
	Vector3 velocity_ = {};	// 速度ベクトル
	bool isDead_ = false; // bulletのデスフラグ
	static const int32_t kLifeTime_ = 60 * 5; // 生きてる時間
	int32_t deathTimer_ = kLifeTime_; // デスタイマー
};
