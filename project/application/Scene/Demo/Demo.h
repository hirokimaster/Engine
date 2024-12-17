/**
* @file Demo.h
* @brief デモシーン
* @author 仁平 琉乃
*/

#pragma once
#include "application/Scene/IScene/IScene.h"
#include "engine/Sprite/Sprite.h"
#include "engine/Model/Model.h"
#include "engine/Input/Input.h"
#include "engine/PostProcess/PostProcess.h"
#include "engine/Model/Animation/ModelAnimation.h"
#include "engine/Object3DPlacer/Object3DPlacer.h"
#include "application/GameManager/GameManager.h"
#include "application/Loader/Loader.h"
#include "application/GameObject/ModelResources/ModelResources.h"
#include "engine/ParticleManager/ParticleManager.h"

class Demo : public IScene {
public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	Demo();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Demo();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize()override;

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update()override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw()override;

	/// <summary>
	/// ポストエフェクト描画
	/// </summary>
	void PostProcessDraw()override;

private:
	Camera camera_;
	std::unique_ptr<Sprite> sprite_ = nullptr;
	uint32_t texHandle_ = 0;
};


