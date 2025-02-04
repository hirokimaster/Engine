/**
* @file ClearScene.h
* @brief クリアシーン
* @author 仁平 琉乃
*/

#pragma once
#include "application/Scene/IScene/IScene.h"
#include "engine/2d/Sprite/Sprite.h"
#include "engine/Input/Input.h"
#include "engine/3d/Object3DPlacer/Object3DPlacer.h"
#include "application/GameObject/Skydome/Skydome.h"
#include "application/SceneTransition/SceneTransition.h"

class ClearScene : public IScene {
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	ClearScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ClearScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize()override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw()override;

	/// <summary>
	/// postProcess描画
	/// </summary>
	void PostProcessDraw()override;

private:
	Camera camera_{};
	std::unique_ptr<Sprite> spriteClear_ = nullptr;
	std::unique_ptr<Sprite> spritePushA_ = nullptr; // pushAのsprite
	uint32_t animationTimer_ = 0;
	std::unique_ptr<Skydome> skydome_ = nullptr;
	bool isTransition_;
	float scaleTimer_ = 0.0f; // claerSpriteのアニメーション用のタイマー
	SceneTransition* sceneTransition_ = nullptr;
};
