#pragma once
#include "engine/Base/WinApp/WinApp.h"
#include "engine/Base/DX/DirectXCommon.h"
#include "engine/Utility/ShaderCompile.h"
#include "engine/GraphicsPipeline/GraphicsPipeline.h"
#include "engine/Utility/ImGuiManager/ImGuiManager.h"
#include "engine/TextureManager/TextureManager.h"
#include "engine/Input/Input.h"
#include "engine/Audio/Audio.h"
#include "application/GameManager/GameManager.h"
#include "engine/SceneFactory/SceneFactory.h"

class Framework {
public:
	virtual ~Framework() = default;
	virtual void Initialize();
	virtual void Finalize();
	virtual void Update();
	virtual void Draw() = 0;
	virtual void PostProcessDraw();

	void Run();

private:
	WinApp* win_;
	DirectXCommon* dxCommon_;
	Audio* audio_;
	ImGuiManager* imguiManager_;
	GameManager* gameManager_ = nullptr;
	std::unique_ptr<AbstractSceneFactory> sceneFactory_ = nullptr;
};
