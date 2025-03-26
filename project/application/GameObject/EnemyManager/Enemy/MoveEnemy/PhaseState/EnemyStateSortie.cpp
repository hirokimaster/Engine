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
	if (player_->GetWorldPosition().z >= pEnemy->GetEventTrigger().z) {
		isSortie_ = true;
		pEnemy->SetIsSortie(isSortie_);
	}
	// ポジションまで移動したら攻撃モードに移る
	if (isSortie_) {
		if (t_ <= 1.0f) {
			t_ += 1.0f / 30.0f;
			move_ = CatmullRomPosition(pEnemy->GetMoveControlPoints_(), t_);
			pEnemy->SetPosition(move_);
		}
		else {
			t_ = 1.0f;
			isSortie_ = false;
			pEnemy->ChangeState(std::make_unique<EnemyStateFire>());
		}
	}

	
}