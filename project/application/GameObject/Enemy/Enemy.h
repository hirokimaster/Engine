#pragma once
#include "engine/Utility/CollisionManager/Collider/Collider.h"
#include "application/GameObject/Enemy/Bullet/EnemyBullet.h"
#include "engine/TextureManager/TextureManager.h"
#include "engine/ParticleSystem/ParticleSystem.h"
#include "engine/Object3DPlacer/Object3DPlacer.h"

class Enemy : public Collider {
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

	static const uint32_t kFireInterval_ = 60; // 発射間隔

private:

	/// <summary>
	/// 当たり判定
	/// </summary>
	void OnCollision()override;

	/// <summary>
	/// 攻撃
	/// </summary>
	void Fire();

	/// <summary>
	/// 弾の更新
	/// </summary>
	void BulletUpdate();

public:

#pragma region getter

	Vector3 GetWorldPosition() const override;

	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() const { return bullets_; }

	bool GetIsDead()const { return isDead_; }


#pragma endregion

#pragma region setter

	void SetPosition(Vector3 position) { worldTransform_.translate = position; }

#pragma endregion

private:
	bool isDead_ = false; // デスフラグ
	std::list<std::unique_ptr<EnemyBullet>> bullets_; // 弾のリスト
	std::list<std::unique_ptr<EnemyBullet>>::iterator bulletsItr_; // 弾のイテレータ
	uint32_t texHandleBullet_ = 0; // bulletのtexHandle
	float fireTimer_ = 120.0f; // 攻撃のタイマー
	WorldTransform worldTransform_{};
	std::unique_ptr<Object3DPlacer> object_ = nullptr;
	static const int32_t kLifeTime_ = 60 * 5; // 生きてる時間
	int32_t deathTimer_ = kLifeTime_; // デスタイマー
};

