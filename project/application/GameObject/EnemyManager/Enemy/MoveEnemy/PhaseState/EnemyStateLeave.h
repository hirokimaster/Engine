#pragma once
#pragma once
/**
* @file EnemyStateLeave.h
* @brief 離脱
* @author 仁平 琉乃
*/

#pragma once
#include "application/GameObject/EnemyManager/Enemy/MoveEnemy/PhaseState/BasePhaseState/BasePhaseStateEnemy.h"
#include <cstdint>

class EnemyStateLeave : public BasePhaseStateEnemy {
public:

	~EnemyStateLeave();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="pEnemy"></param>
	void Update(MoveEnemy* pEnemy)override;


private:


};
