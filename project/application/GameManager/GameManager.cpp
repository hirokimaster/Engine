/**
* @file GameManager.cpp
* @brief ゲームやシーンのマネージャ
* @author 仁平 琉乃
*/

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

		// 調整項目のファイル読み込み
		AdjustmentVariables::GetInstance()->LoadFiles();
		// particleのファイル読み込み
		ParticleManager::GetInstance()->Initialize();

		scene_->Initialize();
	}

	// 更新
	AdjustmentVariables::GetInstance()->Update();
	ParticleManager::GetInstance()->UpdateEditor();
	scene_->Update();
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