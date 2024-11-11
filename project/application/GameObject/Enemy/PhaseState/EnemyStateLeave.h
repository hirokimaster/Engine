#pragma once
#pragma once
/**
* @file EnemyStateLeave.h
* @brief 離脱
* @author 仁平 琉乃
*/

#pragma once
#include "application/GameObject/Enemy/PhaseState/IPhaseState/IPhaseStateEnemy.h"
#include <cstdint>

class EnemyStateLeave : public IPhaseStateEnemy {
public:

	~EnemyStateLeave();

	void Update(Enemy* pEnemy)override;


private:

	bool isSetvelocity_ = false;
};
