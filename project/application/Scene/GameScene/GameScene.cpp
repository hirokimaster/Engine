#include "GameScene.h"
#include "engine/ModelManager/ModelManager.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

void GameScene::Initialize()
{
	camera_.Initialize();
	ModelResources::GetInstance()->LoadModel();
	playerManager_ = std::make_unique<PlayerManager>();
	playerManager_->Initialize();
}

void GameScene::Update()
{
	playerManager_->Update();

	camera_.UpdateMatrix();
}

void GameScene::Draw()
{
	playerManager_->Draw(camera_);
}

void GameScene::PostProcessDraw()
{
}
