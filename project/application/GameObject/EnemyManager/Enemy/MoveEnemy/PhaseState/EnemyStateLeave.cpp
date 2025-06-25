#include "EnemyStateLeave.h"
#include "application/GameObject/EnemyManager/Enemy/MoveEnemy/MoveEnemy.h"
#include "application/GameObject/Player/Player.h"
#include <random>

EnemyStateLeave::~EnemyStateLeave()
{
}

void EnemyStateLeave::Update(MoveEnemy* pEnemy)
{
	// 死ぬ
	pEnemy->SetIsDead(true);
}
