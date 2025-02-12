#pragma once
#include "application/GameState/IGameState.h"
#include "application/Scene/GameScene/GameScene.h"

class StateGameStart : public IGameState {
public:

	void Update(GameScene* pGameScene)override;

private:


};
