/**
* @file ClearScene.h
* @brief クリアシーン
* @author 仁平 琉乃
*/

#pragma once
#include "application/Scene/IScene/IScene.h"
#include "engine/Sprite/Sprite.h"
#include "application/GameManager/GameManager.h"
#include "engine/Input/Input.h"
#include "engine/Object3DPlacer/Object3DPlacer.h"
#include "application/GameObject/Skydome/Skydome.h"

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

private:

	std::unique_ptr<Object3DPlacer> objectPlayer_ = nullptr;
	uint32_t texHandlePlayer_ = 0;
	Camera camera_{};
	WorldTransform worldTransform_{};
	std::unique_ptr<Sprite> spriteClear_ = nullptr;
	uint32_t texHandleClear_ = 0;
	std::unique_ptr<Sprite> spritePushA_ = nullptr; // pushAのsprite
	uint32_t texHandlePushA_ = 0;
	uint32_t animationTimer_ = 0;
	std::unique_ptr<Sprite> spriteFade_;
	Vector4 spriteColor_{};
	uint32_t texHandleWhite_ = 0;
	std::unique_ptr<Skydome> skydome_ = nullptr;
	bool isTransition_;
};
