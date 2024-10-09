#pragma once
#include "application/Scene/IScene/IScene.h"
#include "engine/Sprite/Sprite.h"
#include "application/GameManager/GameManager.h"
#include "engine/Input/Input.h"

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
	std::unique_ptr<Sprite> spriteClear_ = nullptr;
	uint32_t texHandleClear_ = 0;
	std::unique_ptr<Sprite> spritePushA_ = nullptr; // pushAのsprite
	uint32_t texHandlePushA_ = 0;
};
