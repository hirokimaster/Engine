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

	/// <summary>
	/// 攻撃
	/// </summary>
	void Fire()override;

	/// <summary>
	/// 当たり判定
	/// </summary>
	void OnCollision();

#pragma region getter

	Vector3 GetWorldPosition()const override;

	bool GetIsDead()const override { return isDead_; }

#pragma endregion

private:

	// デスフラグ
	bool isDead_;

};
