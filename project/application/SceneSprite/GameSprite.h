/**
* @file GameSprite.h
* @brief ゲームシーンのスプライト
* @author 仁平 琉乃
*/

#pragma once
#include "engine/2d/Sprite/Sprite.h"

class GameSprite {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:
	// UI
	std::unique_ptr<Sprite> spriteAttack_ = nullptr;
	std::unique_ptr<Sprite> spriteMove_ = nullptr;
};
