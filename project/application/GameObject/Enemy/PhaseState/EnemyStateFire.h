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

	void Update(Enemy* pEnemy)override;

private:

	const uint32_t kFireInterval_ = 60;

	uint32_t fireTimer_ = 60;

};
