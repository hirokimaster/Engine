#pragma once
#include "application/GameObject/BaseObject/BaseObject.h"
#include "engine/Utility/CollisionManager/Collider/Collider.h"
#include "application/GameObject/Enemy/Bullet/EnemyBullet.h"
#include "engine/TextureManager/TextureManager.h"

class Enemy : public BaseObject, public Collider{
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

private:
	bool isDead_ = false; // デスフラグ
	std::list<std::unique_ptr<EnemyBullet>> bullets_; // 弾のリスト
	std::list<std::unique_ptr<Object3DPlacer>> objectBullets_;
	std::list<std::unique_ptr<EnemyBullet>>::iterator bulletsItr_; // 弾のイテレータ
	uint32_t texHandleBullet_ = 0; // bulletのtexHandle
	float fireTimer_ = 120.0f; // 攻撃のタイマー
};