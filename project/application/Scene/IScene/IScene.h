#pragma once
#include <memory>

class GameManager;

class IScene
{
protected:

	// ステージナンバー
	static int stageNo_;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() = 0;

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// ポストエフェクト描画
	/// </summary>
	virtual void PostProcessDraw();

	/// <summary>
	/// ゲームマネージャを設定
	/// </summary>
	/// <param name="gameManager"></param>
	virtual void SetGameManager(GameManager* gameManager) { gameManager_ = gameManager; }

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~IScene();

private:

	GameManager* gameManager_ = nullptr;

};
