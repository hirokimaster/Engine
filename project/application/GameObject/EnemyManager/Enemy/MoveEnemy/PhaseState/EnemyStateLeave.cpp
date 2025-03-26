#include "EnemyStateLeave.h"
#include "application/GameObject/EnemyManager/Enemy/MoveEnemy/MoveEnemy.h"
#include "application/GameObject/Player/Player.h"
#include <random>

EnemyStateLeave::~EnemyStateLeave()
{
}

void EnemyStateLeave::Update(MoveEnemy* pEnemy)
{
	// 移動ベクトル
	const Vector3 velocity = {
		5.0f,0.0f,0.0f
	};

	pEnemy->SetVelocity(velocity);
	pEnemy->Move();
}
