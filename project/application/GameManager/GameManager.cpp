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

		transition_ = nullptr;
		scene_ = std::move(nextScene_);
		nextScene_ = nullptr;

		scene_->SetGameManager(this);

		// 調整項目のファイル読み込み
		AdjustmentVariables::GetInstance()->LoadFiles();
		ObjectManager::GetInstance()->Initialize();
		ParticleEditor::GetInstance()->LoadFiles();

		scene_->Initialize();

		// シーン遷移の演出開始
		if (transition_) {
			transition_->Start(nullptr);
		}
	}

	// 更新
	AdjustmentVariables::GetInstance()->Update();
	ParticleManager::GetInstance()->UpdateEditor();
	scene_->Update();

	if (transition_) {
		transition_->Update();
	}
}

void GameManager::Draw()
{
	scene_->Draw();

	if (transition_) {
		transition_->Draw();
	}
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

	if (transition_) {
		// シーン遷移の演出開始
		transition_->Start([this, sceneName]() {
			nextScene_ = sceneFactory_->CreateScene(sceneName);
			});
	}
	// 演出を設定していないなら普通に切り替える
	else {
		nextScene_ = sceneFactory_->CreateScene(sceneName);
	}
	
}