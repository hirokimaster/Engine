#include "EnemyStateFire.h"
#include "application/GameObject/Enemy/Enemy.h"
#include "application/GameObject/Player/Player.h"

EnemyStateFire::~EnemyStateFire()
{

}

void EnemyStateFire::Update(Enemy* pEnemy)
{
	// 発射タイマーをデクリメント
	--fireTimer_;

	if (fireTimer_ <= 0) {
		// 弾を発射
		pEnemy->Fire();
		// 発射タイマーの初期化
		fireTimer_ = kFireInterval_;
	}

	pEnemy->SetVelocity(Vector3(0.0f, 0.0f, 3.0f));
	pEnemy->Move();

}
