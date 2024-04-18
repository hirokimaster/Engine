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
	worldTransform_.scale = { 5.0f,5.0f,5.0f };
	//worldTransform_.translate.y = -5.0f;
	camera_.Initialize();
	texHandle_ = TextureManager::Load("resources/uvChecker.png");
	anim_ = std::make_unique<ModelAnimation>();
	anim_->Initialize("simpleSkin.gltf");
	anim_->SetTexHandle(texHandle_);
	skeleton_ = anim_->CreateSkeleton();
	
}

void GameScene::Update()
{
	animationTime_ += 1.0f / 60.0f;
	anim_->ApplyAnimation(skeleton_, animationTime_);
	anim_->Update(skeleton_);
	//anim_->PlayAnimation();
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
