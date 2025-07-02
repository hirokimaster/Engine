/**
* @file ClaerSprite.h
* @brief クリアシーンのスプライト
* @author 仁平 琉乃
*/
#pragma once
#include "engine/2d/Sprite/Sprite.h"
#include "application/AdjustmentVariables/AdjustmentVariables.h"

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

	/// <summary>
	/// 調整項目の追加
	/// </summary>
	void AddAdjustmentVariables();

	/// <summary>
	/// 調整項目の適用
	/// </summary>
	void ApplyAdjustmentVariables();

private:
	uint32_t animationTimer_ = 0;
	std::unique_ptr<Sprite> spriteClear_ = nullptr;
	Vector2 spriteClearPosition_{};
	std::unique_ptr<Sprite> spritePushA_ = nullptr; // pushAのsprite
	Vector2 spritePushAPosition_{};
};
