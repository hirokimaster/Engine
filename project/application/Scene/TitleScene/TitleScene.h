#pragma once
#include "application/Scene/IScene/IScene.h"
#include "engine/Sprite/Sprite.h"
#include "engine/PostProcess/PostProcess.h"
#include "application/GameManager/GameManager.h"
#include "engine/Input/Input.h"
#include "application/GameObject/ModelResources/ModelResources.h"

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

private:
	std::unique_ptr<PostProcess> postProcess_ = nullptr; // postProcess
	std::unique_ptr<Sprite> spriteTitle_ = nullptr; // Titleのsprite
	uint32_t texHandleTitle_ = 0;
};
