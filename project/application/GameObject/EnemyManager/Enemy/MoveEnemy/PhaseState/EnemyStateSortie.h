/**
* @file EnemyStateSortie.h
* @brief 敵の生成、挙動
* @author 仁平 琉乃
*/

#pragma once
#include "application/GameObject/EnemyManager/Enemy/MoveEnemy/PhaseState/BasePhaseState/BasePhaseStateEnemy.h"
#include "engine/Math/Mathfunction.h"

class EnemyStateSortie : public BasePhaseStateEnemy {
public:

	~EnemyStateSortie();

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="pEnemy"></param>
	void Update(MoveEnemy* pEnemy)override;

private:
	bool isSortie_ = false;	// 出撃してるか
	float moveParam_ = 0.0f; // 媒介変数
	Vector3 move_{}; // 移動ベクトル
};
