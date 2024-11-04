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
		//pEnemy->Fire();
		// 発射タイマーの初期化
		fireTimer_ = kFireInterval_;
	}

	pEnemy;

	//playerとの距離がこの位置に達したら離脱する
	/*float diff = pEnemy->GetWorldPosition().z - player_->GetWorldPosition().z;
	const float limitDist = 0.0f;
	if (diff < limitDist) {
		pEnemy->ChangeState(std::make_unique<EnemyStateLeave>());
	}*/

}
