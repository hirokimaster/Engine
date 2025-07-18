/**
* @file GameSprite.h
* @brief ゲームシーンのスプライト
* @author 仁平 琉乃
*/

#pragma once
#include "engine/2d/Sprite/Sprite.h"
#include "engine/Input/Input.h"
#include "application/GameManager/GameManager.h"
#include "application/GameObject/Player/Player.h"

class GameScene;

class GameSprite {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

#pragma region setter

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	void SetPlayer(Player* player) { player_ = player; }

#pragma endregion

private:

	/// <summary>
	/// 調整項目の追加
	/// </summary>
	void AddAdjustmentVariables();

	/// <summary>
	/// 調整項目の適用
	/// </summary>
	void ApplyAdjustmentVariables();

private:

	/// <summary>
	/// コンティニュー
	/// </summary>
	void Continue();

	/// <summary>
	/// 選択
	/// </summary>
	void ContinueSelect();

private:
	std::unique_ptr<Sprite> spriteYes_;
	Vector2 spriteYesPosition_{};
	std::unique_ptr<Sprite> spriteNo_;
	Vector2 spriteNoPosition_{};
	std::unique_ptr<Sprite> spriteContinue_;
	Vector2 spriteContinuePosition_{};
	Vector4 texColor_{};
	uint32_t selectNo_ = 0;
	GameScene* gameScene_ = nullptr; // ゲームシーンのポインタ
	Player* player_ = nullptr; // playerのポインタ
	std::unique_ptr<Sprite> spriteSelect_;
	Vector2 spriteSelectPosition_{};
	bool isTransitionTitle_ = false; // タイトルに行くかどうか
	std::unique_ptr<ISceneTransition> transition_; // 遷移
};

