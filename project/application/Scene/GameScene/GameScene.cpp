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
	worldTransform_.Initialize();
	camera_.Initialize();
	texHandle_ = TextureManager::Load("resources/AnimatedCube_BaseColor.png");
	anim_ = std::make_unique<ModelAnimation>();
	anim_->Initialize("AnimatedCube.gltf");
	anim_->SetTexHandle(texHandle_);
	
}

void GameScene::Update()
{
	camera_.UpdateMatrix();
	worldTransform_.UpdateMatrix();
}

void GameScene::Draw()
{
	anim_->Draw(worldTransform_, camera_);
}

void GameScene::PostProcessDraw()
{
}
