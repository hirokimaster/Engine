/**
* @file TitleSprite.h
* @brief タイトルシーンで表示するスプライトの生成
* @author 仁平 琉乃
*/

#pragma once
#include "engine/Sprite/Sprite.h"

class TitleSprite {
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

private:
	std::unique_ptr<Sprite> spriteTitle_ = nullptr; // Titleのsprite
	std::unique_ptr<Sprite> spritePushA_ = nullptr; // pushAのsprite
	uint32_t animationTimer_ = 0; // Aボタン点滅用のタイマー
};
