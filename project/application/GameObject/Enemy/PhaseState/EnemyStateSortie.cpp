#include "EnemyStateSortie.h"
#include "application/GameObject/Enemy/Enemy.h"

void EnemyStateSortie::Update(Enemy* pEnemy)
{
	// 座標から見て右か左で速度を変える
	// 右なら
	if (pEnemy->GetWorldPosition().x > 0.0f) {
		velocity_ = { 0.0f,0.0f,0.0f };
	}
	// 左なら
	else if (pEnemy->GetWorldPosition().x < 0.0f) {
		velocity_ = { 0.0f,0.0f,0.0f };
	}

	pEnemy->SetVelocity(velocity_);
	// 移動
	pEnemy->Move();
}