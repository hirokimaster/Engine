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
	postProcess_ = std::make_unique<PostProcess>();
	postProcess_->Initialize();
	postProcess_->SetEffect(LuminanceOutline);

	texHandle_ = TextureManager::Load("resources/uvChecker.png");
	sprite_.reset(Sprite::Create(texHandle_));
}

void GameScene::Update()
{
}

void GameScene::Draw()
{
	postProcess_->Draw();
}

void GameScene::PostProcessDraw()
{
	postProcess_->PreDraw();

	sprite_->Draw();

	postProcess_->PostDraw();
}
