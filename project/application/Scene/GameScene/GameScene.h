#pragma once
#include "application/Scene/IScene/IScene.h"
#include "engine/Sprite/Sprite.h"
#include "engine/Model/Model.h"
#include "engine/Input/Input.h"
#include "engine/PostProcess/PostProcess.h"
#include "engine/Model/Animation/ModelAnimation.h"
#include "engine/Object3DPlacer/Object3DPlacer.h"
#include "engine/Model/SkyBox/SkyBox.h"
#include "engine/Lighting/Lighting.h"

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
	Camera camera_{};
	WorldTransform worldTransform_{};
	std::unique_ptr<Object3DPlacer>	anim_;
	float animationTime_ = 0;
	uint32_t texHandle_ = 0;
	const float kMoveSpeed_ = 0.05f;
	bool isAnimation_ = false;

	/*std::unique_ptr<SkyBox> skyBox_;
	uint32_t texHandle_ = 0;
	WorldTransform transform_{};
	Camera camera_{};

	std::unique_ptr<Object3DPlacer> object_;
	std::unique_ptr<Lighting> light_;
	WorldTransform worldTransform_{};
	uint32_t texHandle2_ = 0;
	Material material_{};*/
};
