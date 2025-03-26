#pragma once
#include <list>
#include "application/GameObject/EnemyManager/Enemy/IEnemy.h"

class EnemyManager {
public:
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	void Draw(const Camera& camera);

	/// <summary>
	/// リストに敵追加
	/// </summary>
	void EnemyPush(std::unique_ptr<IEnemy> enemy) { enemys_.push_back(std::move(enemy)); }

#pragma region setter

#pragma endregion

private:
	// enemyのリスト
	std::list<std::unique_ptr<IEnemy>> enemys_;
};