#include "PlayerManager.h"

void PlayerManager::Initialize()
{
	player_ = std::make_unique<Player>();
	objectPlayer_ = std::make_unique<Object3DPlacer>();
	texHandlePlayer_ = TextureManager::Load("resources/white.png");
	player_->Initialize(objectPlayer_.get(), texHandlePlayer_, "cube.obj");
}

void PlayerManager::Update()
{
	player_->Update();
}

void PlayerManager::Draw(Camera& camera)
{
	player_->Draw(camera);
}
