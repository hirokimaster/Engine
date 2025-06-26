/**
* @file EnemyStateSortie.cpp
* @brief 敵の生成、挙動
* @author 仁平 琉乃
*/

#include "EnemyStateSortie.h"
#include "application/GameObject/EnemyManager/Enemy/MoveEnemy/MoveEnemy.h"
#include "application/GameObject/Player/Player.h"

EnemyStateSortie::~EnemyStateSortie()
{
}

void EnemyStateSortie::Update(MoveEnemy* pEnemy)
{
	// 距離
	float diff = pEnemy->GetWorldPosition().z - player_->GetWorldPosition().z;
	if (diff <= 5000.0f) {
		isSortie_ = true;
		pEnemy->SetIsSortie(isSortie_);
	}
	// 攻撃モードに移る
	if (isSortie_) {
		pEnemy->ChangeState(std::make_unique<EnemyStateFire>());
	}
}