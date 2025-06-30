#pragma once
/**
* @file EnemyStateFire.h
* @brief 攻撃してくる状態
* @author 仁平 琉乃
*/

#pragma once
#include "application/GameObject/EnemyManager/Enemy/MoveEnemy/PhaseState/BasePhaseState/BasePhaseStateEnemy.h"
#include "application/GameObject/EnemyManager/Enemy/MoveEnemy/PhaseState/EnemyStateLeave.h"
#include <cstdint>

class EnemyStateFire : public BasePhaseStateEnemy {
public:

	~EnemyStateFire();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="pEnemy"></param>
	void Update(MoveEnemy* pEnemy)override;

	// 攻撃間隔
	const uint32_t kFireInterval_ = 120;

private:

	uint32_t fireTimer_ = 90; // 攻撃タイマー

};
