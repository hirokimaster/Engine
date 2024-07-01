#include "EnemyManager.h"

void EnemyManager::Initialize()
{
	enemy_ = std::make_unique<Enemy>();
	objectEnemy_ = std::make_unique<Object3DPlacer>();
	texHandleEnemy_ = TextureManager::Load("resources/white.png");
	enemy_->Initialize(objectEnemy_.get(), texHandleEnemy_, "cube.obj");
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
