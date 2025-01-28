/**
* @file EnemyStateSortie.h
* @brief 敵の生成、挙動
* @author 仁平 琉乃
*/

#pragma once
#include "application/GameObject/Enemy/PhaseState/BasePhaseState/BasePhaseStateEnemy.h"

class EnemyStateSortie : public BasePhaseStateEnemy {
public:

	~EnemyStateSortie();

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="pEnemy"></param>
	void Update(Enemy* pEnemy)override;

private:
	bool isSortie_ = false;	// 出撃してるか
	float t_ = 0.0f; // 媒介変数
};
