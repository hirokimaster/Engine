#include "EnemyStateLeave.h"
#include "application/GameObject/Enemy/Enemy.h"
#include "application/GameObject/Player/Player.h"
#include <random>

EnemyStateLeave::~EnemyStateLeave()
{
}

void EnemyStateLeave::Update(Enemy* pEnemy)
{
	// 移動ベクトル
	const Vector3 velocity = {
		5.0f,0.0f,0.0f
	};

	pEnemy->SetVelocity(velocity);
	pEnemy->Move();
}
