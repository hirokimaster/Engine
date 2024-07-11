#include "Engine.h"

/// <summary>
/// 初期化
/// </summary>
void Engine::Initialize() {
	Framework::Initialize();
	// シーンの初期化
	sceneFactory_ = std::make_unique<SceneFactory>();
	gameManager_ = GameManager::GetInstance();
	gameManager_->SetSceneFactory(std::move(sceneFactory_));
	gameManager_->ChangeScene("GAME");

}

void Engine::Update()
{
	Framework::Update();
}

void Engine::Draw()
{
	gameManager_->Draw();
}

/// <summary>
/// 終了
/// </summary>
void Engine::Finalize() {

	Framework::Finalize();
}