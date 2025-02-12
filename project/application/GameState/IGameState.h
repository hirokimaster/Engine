#pragma once

class GameScene;

class IGameState {
public:
	/// <summary>
	/// 仮想デストラクタ
	/// </summary>
	virtual ~IGameState() = 0;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update(GameScene* pGameScene) = 0;

private:

};
