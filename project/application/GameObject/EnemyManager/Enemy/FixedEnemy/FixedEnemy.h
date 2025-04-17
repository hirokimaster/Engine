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

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	void Draw(const Camera& camera)override;

#pragma region getter

	Vector3 GetWorldPosition()const override;

	bool GetIsDead()const override { return isDead_; }

	Collider* GetCollider()override { return collider_.get(); }

#pragma endregion

#pragma region setter

	void SetPlayer(Player* player) { player_ = player; }

	void SetBulletObjectPool(BulletObjectPool* ptr) { bulletObjectPool_ = ptr; }

	void SetPosition(const Vector3& position)override { object_->SetPosition(position); }

	void SetScale(const Vector3& scale) { object_->SetScale(scale); }

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
	std::unique_ptr<PlaneProjectionShadow> shadow_; // 影
};
