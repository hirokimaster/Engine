#pragma once
#include <memory>

class GameManager;

class IScene
{
protected:

	// ステージナンバー
	static int stageNo_;

public:

	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void PostProcessDraw();
	virtual void SetGameManager(GameManager* gameManager) { gameManager_ = gameManager; }

	virtual ~IScene();

private:

	GameManager* gameManager_ = nullptr;

};
