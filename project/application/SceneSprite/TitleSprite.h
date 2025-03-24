/**
* @file TitleSprite.h
* @brief タイトルシーンで表示するスプライトの生成
* @author 仁平 琉乃
*/

#pragma once
#include "engine/2d/Sprite/Sprite.h"
#include "application/AdjustmentVariables/AdjustmentVariables.h"

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

	/// <summary>
	/// 調整項目の追加
	/// </summary>
	void AddAdjustmentVariables();

	/// <summary>
	/// 調整項目の適用
	/// </summary>
	void ApplyAdjustmentVariables();

private:
	std::unique_ptr<Sprite> spriteTitle_ = nullptr; // Titleのsprite
	Vector2 spriteTitlePosition_{};
	std::unique_ptr<Sprite> spritePushA_ = nullptr; // pushAのsprite
	Vector2 spritePushAPosition_{};
	uint32_t animationTimer_ = 0; // Aボタン点滅用のタイマー
};
