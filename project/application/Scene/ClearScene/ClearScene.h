/**
* @file ClearScene.h
* @brief クリアシーン
* @author 仁平 琉乃
*/

#pragma once
#include "application/Scene/IScene/IScene.h"
#include "engine/Input/Input.h"
#include "engine/3d/Object3dPlacer/Object3dPlacer.h"
#include "application/GameObject/Skydome/Skydome.h"
#include "application/SceneTransition/FadeIn/FadeIn.h"
#include "application/SceneTransition/FadeOut/FadeOut.h"
#include "application/GameManager/GameManager.h"
#include "application/SceneSprite/ClearSprite.h"
#include "application/GameObject/CameraManager/CameraManager.h"

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
	CameraManager* cameraManager_ = nullptr;
	std::unique_ptr<Skydome> skydome_ = nullptr;
	bool isTransition_ = false;
	std::unique_ptr<ISceneTransition> transition_ = nullptr;
	std::unique_ptr<ClearSprite> sprite_ = nullptr;
};
