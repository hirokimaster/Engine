#include "EnemyStateSortie.h"
#include "application/GameObject/Enemy/Enemy.h"

EnemyStateSortie::~EnemyStateSortie()
{
}

void EnemyStateSortie::Update(Enemy* pEnemy)
{
	// 速度ベクトル
	Vector3 velocity = { 0.0f,0.0f,0.0f };

	// 座標から見て右か左で速度を変える
	// 右なら
	if (pEnemy->GetWorldPosition().x > 0.0f) {
		velocity = { 0.0f,0.0f,0.0f };
	}
	// 左なら
	else if (pEnemy->GetWorldPosition().x < 0.0f) {
		velocity = { 0.0f,0.0f,0.0f };
	}

	pEnemy->SetVelocity(velocity);
	// 移動
	pEnemy->Move();
}