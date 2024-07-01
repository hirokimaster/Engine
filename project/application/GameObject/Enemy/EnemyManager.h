#pragma once
#include "application/GameObject/Enemy/Enemy.h"
#include "engine/TextureManager/TextureManager.h"
#include "engine/Utility/CollisionManager/CollisionManager.h"

class EnemyManager {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	void Draw(Camera& camera);

	/// <summary>
	/// colliderをリストに登録
	/// </summary>
	void ColliderPush(CollisionManager* collision);

private:
	std::list<std::unique_ptr<Enemy>> enemys_;
	std::list<std::unique_ptr<Enemy>>::iterator enemysItr_;
	std::list<std::unique_ptr<Object3DPlacer>> objectEnemys_;;
	uint32_t texHandleEnemy_ = 0;
};
