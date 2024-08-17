#pragma once
#include "engine/Utility/CollisionManager/Collider/Collider.h"
#include "engine/Object3DPlacer/Object3DPlacer.h"
#include <random>

enum BulletType {
	Normal,
	Spiral,
	Missile,
	Radial
};

class Player;

class EnemyBullet : public Collider {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="object"></param>
	/// <param name="texHandle"></param>
	/// <param name="model"></param>
	void Initialize(uint32_t texHandle, BulletType type);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(BulletType type);

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

	/// <summary>
	/// 弾がどう出るかランダムで出すやつ
	/// </summary>
	/// <returns></returns>
	Vector3 RandomDirection();

	/// <summary>
	/// カーブの計算
	/// </summary>
	/// <param name="dt"></param>
	/// <returns></returns>
	Vector3 CalculateCurve(float dt);

	/// <summary>
	/// 
	/// </summary>
	void CurveBullet();



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

	void SetPlayer(Player* player) { player_ = player; }

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
	Player* player_ = nullptr;
	float deltaTime_ = 0.0f;
	float timeElapsed_ = 0.0f; // 経過時間
};
