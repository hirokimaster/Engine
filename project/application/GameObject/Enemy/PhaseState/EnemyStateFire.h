#pragma once
/**
* @file EnemyStateFire.h
* @brief 攻撃してくる状態
* @author 仁平 琉乃
*/

#pragma once
#include "application/GameObject/Enemy/PhaseState/IPhaseState/IPhaseStateEnemy.h"
#include "application/GameObject/Enemy/PhaseState/EnemyStateLeave.h"
#include <cstdint>

class EnemyStateFire : public IPhaseStateEnemy {
public:

	~EnemyStateFire();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="pEnemy"></param>
	void Update(Enemy* pEnemy)override;

	// 攻撃間隔
	const uint32_t kFireInterval_ = 60;

private:

	uint32_t fireTimer_ = 60; // 攻撃タイマー

};
