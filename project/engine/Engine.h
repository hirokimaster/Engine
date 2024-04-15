#pragma once
#include "engine/Framework/Framework.h"

class Engine : public Framework {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize()override;

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize()override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update()override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw()override;



private:

	GameManager* gameManager_ = nullptr;
	std::unique_ptr<SceneFactory> sceneFactory_ = nullptr;
};