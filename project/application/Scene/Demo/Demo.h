/**
* @file Demo.h
* @brief デモシーン
* @author 仁平 琉乃
*/

#pragma once
#include "application/Scene/IScene/IScene.h"
#include "engine/2d/Sprite/Sprite.h"
#include "engine/3d/Model/Model.h"
#include "engine/Input/Input.h"
#include "engine/Graphics/PostProcess/PostProcess.h"
#include "engine/3d/Model/Animation/ModelAnimation.h"
#include "engine/3d/Object3DPlacer/Object3DPlacer.h"
#include "application/GameManager/GameManager.h"
#include "application/Loader/Loader.h"
#include "application/GameObject/ModelResources/ModelResources.h"
#include "engine/3d/Line/Line.h"
#include "engine/3d/ObjectManager/ObjectManager.h"

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
	std::unique_ptr<ObjectManager> objectManager_;
	std::weak_ptr<Object3dData> object_;
	std::weak_ptr<Object3dData> object2_;
};


