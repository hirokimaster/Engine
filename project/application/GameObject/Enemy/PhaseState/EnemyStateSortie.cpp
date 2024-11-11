/**
* @file EnemyStateSortie.cpp
* @brief 敵の生成、挙動
* @author 仁平 琉乃
*/

#include "EnemyStateSortie.h"
#include "application/GameObject/Enemy/Enemy.h"
#include "application/GameObject/Player/Player.h"

EnemyStateSortie::~EnemyStateSortie()
{
}

void EnemyStateSortie::Update(Enemy* pEnemy)
{
	Vector3 playerPosition = player_->GetWorldPosition();
	Vector3 enemyPosition = pEnemy->GetWorldPosition();
	float diffZ = enemyPosition.z - playerPosition.z;
	if (diffZ <= 0.0f) {
		isSortie_ = true;
		pEnemy->SetIsSortie(true);
	}

	if (isSortie_) {
		if (t_ <= 1.0f) {
			Vector3 sortiePosition = Lerp(pEnemy->GetStartPosition(), pEnemy->GetEndPosition(), t_);
			pEnemy->SetPosition(sortiePosition);
			t_ += 0.01f;
		}
		else {
			isSortie_ = false;
			pEnemy->ChangeState(std::make_unique<EnemyStateFire>());
		}
	}
}