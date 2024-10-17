#pragma once
#include "application/GameObject/Enemy/PhaseState/IPhaseState/IPhaseStateEnemy.h"

class EnemyStateSortie : public IPhaseStateEnemy {
public:

	void Update(Enemy* pEnemy)override;

private:

	Vector3 velocity_{}; // 速度ベクトル
};
