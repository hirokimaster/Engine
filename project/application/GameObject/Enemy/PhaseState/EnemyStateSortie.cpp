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
	

	// ポジションまで移動したら攻撃モードに移る
	if (isSortie_) {
		if (t_ <= 1.0f) {
			
		}
		else {
			isSortie_ = false;
			pEnemy->ChangeState(std::make_unique<EnemyStateFire>());
		}
	}
}