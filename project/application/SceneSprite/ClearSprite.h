/**
* @file ClaerSprite.h
* @brief クリアシーンのスプライト
* @author 仁平 琉乃
*/
#pragma once
#include "engine/2d/Sprite/Sprite.h"

class ClearSprite {
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
	uint32_t animationTimer_ = 0;
	float scaleTimer_ = 0.0f; // claerSpriteのアニメーション用のタイマー
	std::unique_ptr<Sprite> spriteClear_ = nullptr;
	std::unique_ptr<Sprite> spritePushA_ = nullptr; // pushAのsprite
};
