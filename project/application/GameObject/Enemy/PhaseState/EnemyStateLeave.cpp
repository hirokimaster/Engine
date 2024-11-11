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
	std::random_device rd;  // 非決定論的な乱数生成器
	std::mt19937 gen(rd()); // メルセンヌ・ツイスタによる乱数生成器
	std::uniform_int_distribution<int> dist(0, 1); // 0または1を生成

	// 2つの移動量の選択肢を定義
	std::vector<Vector3> velocities = {
		{ -0.5f, 0.1f, 0.2f }, // 1つ目の移動量
		{ 0.5f, 0.1f, 0.2f } // 2つ目の移動量
	};

	// ランダムに選択して移動量を設定
	if (!isSetvelocity_) {
		uint32_t randomIndex = dist(gen);
		pEnemy->SetVelocity(velocities[randomIndex]);
		isSetvelocity_ = true;
	}
	
	pEnemy->Move();
}
