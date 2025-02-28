#include "EnemyStateFire.h"
#include "application/GameObject/Enemy/Enemy.h"
#include "application/GameObject/Player/Player.h"

EnemyStateFire::~EnemyStateFire()
{

}

void EnemyStateFire::Update(Enemy* pEnemy)
{
	// 差分もとめる
	Vector3 playerPosition = player_->GetWorldPosition();
	Vector3 enemyPosition = pEnemy->GetWorldPosition();

	// 発射タイマーをデクリメント
	--fireTimer_;

	if (fireTimer_ <= 0) {
		// 弾を発射
		pEnemy->Fire();
		// 発射タイマーの初期化
		fireTimer_ = kFireInterval_;
	}

	pEnemy->Move();

}
