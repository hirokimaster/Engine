/**
* @file TitleScene.h
* @brief タイトルシーン
* @author 仁平 琉乃
*/

#pragma once
#include "application/Scene/IScene/IScene.h"
#include "engine/Input/Input.h"
#include "application/GameObject/ModelResources/ModelResources.h"
#include "application/GameObject/Skydome/Skydome.h"
#include "application/SceneTransition/FadeIn/FadeIn.h"
#include "application/SceneSprite/TitleSprite.h"
#include "application/GameManager/GameManager.h"
#include <application/PostEffect/PostEffect.h>
#include "application/GameObject/CameraManager/CameraManager.h"

class TitleScene : public IScene {
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	TitleScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TitleScene();

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
	/// postEffect描画
	/// </summary>
	void PostProcessDraw()override;

private:
	/// <summary>
	/// textureはここに書く
	/// </summary>
	void LoadTextureFile();

private:
	std::unique_ptr<PostEffect> postEffect_ = nullptr; // postEffect
	std::unique_ptr<Object3DPlacer> objectPlayer_ = nullptr;
	CameraManager* cameraManager_ = nullptr;
	std::unique_ptr<TitleSprite> titleSprite_ = nullptr;
	
	// シーン遷移用
	bool isTransition_ = false;
	std::unique_ptr<ISceneTransition> transition_ = nullptr;
	std::unique_ptr<Skydome> skydome_ = nullptr;
	// particle
	ParticleManager* particleManager_ = nullptr;
};
