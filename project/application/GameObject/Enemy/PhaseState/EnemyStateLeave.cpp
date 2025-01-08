#include "EnemyStateLeave.h"
#include "application/GameObject/Enemy/Enemy.h"
#include "application/GameObject/Player/Player.h"
#include <random>

EnemyStateLeave::~EnemyStateLeave()
{
}

void EnemyStateLeave::Update(Enemy* pEnemy)
{
	// ランダム生成器の初期化
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dist(0, 1);

	std::vector<Vector3> velocities = {
		{ -0.5f, 0.1f, 0.2f }, 
		{ 0.5f, 0.1f, 0.2f }
	};

	// ランダムに選択して移動量を設定
	if (!isSetvelocity_) {
		uint32_t randomIndex = dist(gen);
		pEnemy->SetVelocity(velocities[randomIndex]);
		isSetvelocity_ = true;
	}
	
	pEnemy->Move();

	--deadTimer_;
	if (deadTimer_ <= 0.0f) {
		pEnemy->SetIsDead(true);
	}
}
