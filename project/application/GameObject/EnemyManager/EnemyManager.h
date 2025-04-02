#pragma once
#include <list>
#include "application/GameObject/EnemyManager/Enemy/IEnemy.h"
#include "application/Loader/Loader.h"
#include "application/GameObject/EnemyManager/Enemy/MoveEnemy/MoveEnemy.h"
#include "application/GameObject/EnemyManager/Enemy/FixedEnemy/FixedEnemy.h"

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
	void Draw(const Camera& camera);

#pragma region getter

	const std::list<std::unique_ptr<IEnemy>>& GetEnemys()const { return enemys_; }

#pragma endregion

#pragma region setter

	void SetLoader(Loader* ptr) { loader_ = ptr; }

	void SetPlayer(Player* ptr) { player_ = ptr; }

	void SetBulletObjectPool(BulletObjectPool* ptr) { bulletObjectPool_ = ptr; }

#pragma endregion

private:
	// enemyのリスト
	std::list<std::unique_ptr<IEnemy>> enemys_;
	// loader
	Loader* loader_ = nullptr;
	// player
	Player* player_ = nullptr;
	// bulletObjectPool
	BulletObjectPool* bulletObjectPool_ = nullptr;
	// パーティクルマネージャ
	ParticleManager* particleManager_ = nullptr;
};