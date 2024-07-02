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
	for (enemysItr_ = enemys_.begin();
		enemysItr_ != enemys_.end(); ++enemysItr_) {

		(*enemysItr_)->Update();
	}

	EnemysDead(); // デスフラグが立ったらリストから削除する
	
}

void EnemyManager::Draw(Camera& camera)
{
	for (enemysItr_ = enemys_.begin();
		enemysItr_ != enemys_.end(); ++enemysItr_) {

		(*enemysItr_)->Draw(camera);
	}
}

void EnemyManager::ColliderPush(CollisionManager* collision)
{
	for (enemysItr_ = enemys_.begin();
		enemysItr_ != enemys_.end(); ++enemysItr_) {

		collision->ColliderPush((*enemysItr_).get()); // enemyをリストに登録
	}
	
}

void EnemyManager::EnemysDead()
{
	std::list<std::unique_ptr<Object3DPlacer>>::iterator objectEnemysItr = objectEnemys_.begin();	// objectのイテレータ

	// デスフラグが立ったら要素を削除
	enemys_.remove_if([&objectEnemysItr, this](std::unique_ptr<Enemy>& enemy) {
		if (enemy->GetIsDead()) {
			// 対応するbulletObjectを削除
			objectEnemysItr = objectEnemys_.erase(objectEnemysItr);
			return true;
		}
		else {
			++objectEnemysItr;
			return false;
		}
		});

}
