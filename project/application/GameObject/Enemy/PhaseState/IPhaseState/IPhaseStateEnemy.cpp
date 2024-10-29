#include "IPhaseStateEnemy.h"

IPhaseStateEnemy::~IPhaseStateEnemy()
{
}

void IPhaseStateEnemy::SetPlayer(Player* player)
{
	player_ = player;
}
