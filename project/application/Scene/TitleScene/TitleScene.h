/**
* @file TitleScene.h
* @brief タイトルシーン
* @author 仁平 琉乃
*/

#pragma once
#include "application/Scene/IScene/IScene.h"
#include "engine/Sprite/Sprite.h"
#include "engine/PostProcess/PostProcess.h"
#include "application/GameManager/GameManager.h"
#include "engine/Input/Input.h"
#include "application/GameObject/ModelResources/ModelResources.h"
#include "engine/Object3DPlacer/Object3DPlacer.h"
#include "application/GameObject/FollowCamera/FollowCamera.h"
#include "application/GameObject/Skydome/Skydome.h"
#include "application/GameObject/SceneTransition/SceneTransition.h"

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
	std::unique_ptr<PostProcess> postProcess_ = nullptr; // postProcess
	std::unique_ptr<Sprite> spriteTitle_ = nullptr; // Titleのsprite
	uint32_t texHandleTitle_ = 0;
	std::unique_ptr<Sprite> spritePushA_ = nullptr; // pushAのsprite
	uint32_t texHandlePushA_ = 0;
	std::unique_ptr<Object3DPlacer> objectPlayer_ = nullptr;
	uint32_t texHandlePlayer_ = 0;
	Camera camera_{};
	WorldTransform worldTransform_{};
	std::unique_ptr<FollowCamera> followCamera_ = nullptr;
	uint32_t texHandleMask_ = 0;
	DissolveParam param_{};
	std::unique_ptr<Sprite> spriteWhite_;
	uint32_t texHandleWhite_ = 0;
	bool isDissolve_ = false;
	bool isDissolve2_ = false;
	uint32_t animationTimer_ = 0;

	// シーン遷移用
	bool isTransition_ = false;
	SceneTransition* sceneTransition_ = nullptr;
	std::unique_ptr<Skydome> skydome_ = nullptr;
};
