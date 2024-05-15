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
	std::unique_ptr<ModelAnimation>	anim_;
	float animationTime_ = 0;
	WorldTransform worldTransform_{};
	Camera camera_{};
	uint32_t texHandle_ = 0;
	std::unique_ptr<Object3DPlacer> object_;
	WorldTransform worldTransform_2{};
	/*uint32_t texHandle_ = 0;
	std::unique_ptr<PostProcess> postProcess_ = nullptr;
	std::unique_ptr<Sprite> sprite_ = nullptr;*/
};
