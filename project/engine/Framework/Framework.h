/**
* @file Framework.h
* @brief 汎用データの初期化を行う（ゲームの固有処理は書かない）
* @author 仁平 琉乃
*/

#pragma once
#include "engine/Base/WinApp/WinApp.h"
#include "engine/Base/DX/DirectXCommon.h"
#include "engine/Utility/ShaderCompile.h"
#include "engine/Graphics/Pipeline/GraphicsPipeline.h"
#include "engine/Utility/ImGuiManager/ImGuiManager.h"
#include "engine/Graphics/TextureManager/TextureManager.h"
#include "engine/Input/Input.h"
#include "engine/Audio/Audio.h"
#include "application/GameManager/GameManager.h"
#include "engine/SceneFactory/SceneFactory.h"
#include "engine/Graphics/Pipeline/ComputePipeline.h"
#include "engine/2d/Sprite/Sprite.h"

class Framework {
public:
	/// <summary>
	///	 デストラクタ
	/// </summary>
	virtual ~Framework() = default;

	/// <summary>
	///  初期化
	/// </summary>
	virtual void Initialize();

	/// <summary>
	///  終了処理
	/// </summary>
	virtual void Finalize();

	/// <summary>
	///  更新処理
	/// </summary>
	virtual void Update();

	/// <summary>
	///  描画
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	///  ポストエフェクト描画
	/// </summary>
	virtual void PostProcessDraw();

	/// <summary>
	///  ループ
	/// </summary>
	void Run();

private:
	WinApp* win_ = nullptr;
	DirectXCommon* dxCommon_ = nullptr;
	Audio* audio_ = nullptr;
	ImGuiManager* imguiManager_ = nullptr;
	GameManager* gameManager_ = nullptr;
	Input* input_ = nullptr;
	GraphicsPipeline* graphicsPipeline_ = nullptr;
	ComputePipeline* computePipeline_ = nullptr;
	std::unique_ptr<AbstractSceneFactory> sceneFactory_ = nullptr;
};
