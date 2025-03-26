#include "EnemyManager.h"

void EnemyManager::Update()
{
	for (auto& enemy : enemys_) {
		enemy->Update();
	}
}

void EnemyManager::Draw(const Camera& camera)
{
	for (auto& enemy : enemys_) {
		enemy->Draw(camera);
	}
}
