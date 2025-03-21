#pragma once
/**
* @file FadeIn.h
* @brief フェードイン
* @author 仁平 琉乃
*/

#include "engine/2d/Sprite/Sprite.h"
#include "application/SceneTransition/ISceneTransition.h"

class FadeIn : public ISceneTransition
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize()override;

	/// <summary>
	/// 開始
	/// </summary>
	void Start(std::function<void()> call)override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw()override;

private:
	std::unique_ptr<Sprite> spriteWhite_ = nullptr;
	float alpha_;
	bool isActive_;
	std::function<void()> onComplete_;
};

