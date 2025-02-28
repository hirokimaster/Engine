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
	/// コンティニュー
	/// </summary>
	void Continue();

private:
	std::unique_ptr<Sprite> spriteYes_;
	std::unique_ptr<Sprite> spriteNo_;
	std::unique_ptr<Sprite> spriteContinue_;
	Vector4 texColor_{};
	uint32_t selectNo_ = 0;
	GameScene* gameScene_ = nullptr; // ゲームシーンのポインタ
	Player* player_ = nullptr; // playerのポインタ
};
