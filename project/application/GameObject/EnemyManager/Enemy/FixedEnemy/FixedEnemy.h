#pragma once
#include "application/GameObject/EnemyManager/Enemy/IEnemy.h"
#include "application/GameObject/Bullet/BulletObjectPool/BulletObjectPool.h"

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

#pragma endregion

#pragma region setter

	void SetPlayer(Player* player) { player_ = player; }

	void SetBulletObjectPool(BulletObjectPool* ptr) { bulletObjectPool_ = ptr; }

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
};
