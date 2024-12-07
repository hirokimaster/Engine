/**
* @file Engine.cpp
* @brief フレームワークとゲームマネージャを呼び出す
* @author 仁平 琉乃
*/

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
	gameManager_->ChangeScene("TITLE");

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

	gameManager_->Finalize();
	Framework::Finalize();
}