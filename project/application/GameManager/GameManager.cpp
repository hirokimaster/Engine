#include "GameManager.h"
#include "application/Scene/GameScene/GameScene.h"
#include <cassert>

GameManager* GameManager::GetInstance()
{
	static GameManager instance;
	return &instance;
}

void GameManager::Update() {

	if (nextScene_) {

		scene_ = std::move(nextScene_);
		nextScene_ = nullptr;

		scene_->SetGameManager(this);

		scene_->Initialize();
	}

	scene_->Update();
}

void GameManager::Initialize()
{
	scene_->Initialize();
}

void GameManager::Draw()
{
	scene_->Draw();
}

void GameManager::PostProcessDraw()
{
	scene_->PostProcessDraw();
}

void GameManager::Finalize()
{

}

void GameManager::ChangeScene(const std::string& sceneName)
{
	assert(sceneFactory_);
	assert(nextScene_ == nullptr);

	nextScene_ = sceneFactory_->CreateScene(sceneName);
}