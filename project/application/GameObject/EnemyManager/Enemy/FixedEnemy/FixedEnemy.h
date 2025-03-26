#pragma once
#include "application/GameObject/EnemyManager/Enemy/IEnemy.h"

class FixedEnemy : public IEnemy {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	void Draw(const Camera& camera);

#pragma region getter

	Vector3 GetWorldPosition()const override;

	bool GetIsDead()const override { return isDead_; }

#pragma endregion

#pragma region setter

	void SetPlayer(Player* player) { player_ = player; }

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
	// 弾のリスト
	std::list<std::unique_ptr<EnemyBullet>> bullets_;
	// 弾のスピード
	float bulletSpeed_ = 0.0f;
};
