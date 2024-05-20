#pragma once
#include "application/Scene/IScene/IScene.h"
#include "engine/Sprite/Sprite.h"
#include "engine/Model/Model.h"
#include "engine/Input/Input.h"
#include "engine/PostProcess/PostProcess.h"
#include "engine/Model/Animation/ModelAnimation.h"
#include "engine/Object3DPlacer/Object3DPlacer.h"

class GameScene : public IScene {
public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

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
	/*std::unique_ptr<Object3DPlacer>	anim_;
	std::unique_ptr<Object3DPlacer> anim_2;
	std::unique_ptr<Object3DPlacer> anim_3;
	float animationTime_ = 0;
	float animationTime_2 = 0;
	WorldTransform worldTransform_{};
	WorldTransform worldTransform_3{};
	Camera camera_{};
	uint32_t texHandle_ = 0;

	WorldTransform worldTransform_2{};*/
	/*uint32_t texHandle_ = 0;
};
