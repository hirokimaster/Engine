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
	// シーンのはじめ(シーンが切り替わった時)srvを全部解放する(texture以外)
	uint32_t srvIndex = SrvManager::GetInstance()->GetIndex();
	std::queue<uint32_t> textureHandleIndices = SrvManager::GetInstance()->GetTextureHandleIndices();
	for (uint32_t i = 1; i < srvIndex; ++i) {
		for (uint32_t j = 0; textureHandleIndices.size(); ++j) {
			if (~i == j) {
				SrvManager::GetInstance()->Free(i);
			}
		}
	}
	
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