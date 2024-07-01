#include "EnemyManager.h"

void EnemyManager::Initialize()
{
	std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>();
	std::unique_ptr<Object3DPlacer> objectEnemy = std::make_unique<Object3DPlacer>();
	objectEnemys_.push_back(std::move(objectEnemy));
	texHandleEnemy_ = TextureManager::Load("resources/white.png");
	enemy->Initialize(objectEnemys_.back().get(), texHandleEnemy_, "cube.obj");
	enemys_.push_back(std::move(enemy));
}

void EnemyManager::Update()
{
	enemy_->Update();
}

void EnemyManager::Draw(Camera& camera)
{
	enemy_->Draw(camera);
}

void EnemyManager::ColliderPush(CollisionManager* collision)
{
	collision->ColliderPush(enemy_.get()); // enemyをリストに登録
}
