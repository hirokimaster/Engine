#pragma once
#include "application/Scene/IScene/IScene.h"
#include "engine/Sprite/Sprite.h"
#include "engine/Model/Model.h"
#include "engine/Input/Input.h"
#include "engine/PostProcess/PostProcess.h"
#include "engine/Model/Animation/ModelAnimation.h"
#include "engine/Object3DPlacer/Object3DPlacer.h"
#include "application/GameManager/GameManager.h"
#include "engine/Loader/Loader.h"

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

	/*uint32_t texHandle_ = 0;
	uint32_t texHandleUV_ = 0;
	uint32_t maskTex_ = 0;
	std::unique_ptr<PostProcess> postProcess_ = nullptr;
	std::unique_ptr<Sprite> sprite_ = nullptr;
	std::unique_ptr<Object3DPlacer> object_ = nullptr;
	Camera camera_{};
	WorldTransform trans_{};
	RandomParam param_{};
	bool dFlag_1 = true;
	bool dFlag_2 = false;
	DissolveParam DissolvePram_{};*/
	std::unique_ptr<Loader> loader_;
	LevelData* levelData_ = nullptr;
	uint32_t texHandle_ = 0;
	Camera camera_;
};


