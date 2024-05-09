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
	worldTransform_.translate.y = -5.0f;
	worldTransform_.rotate.y = -3.142f;
	camera_.Initialize();
	
	anim_ = std::make_unique<ModelAnimation>();
	anim_->Initialize("walk.gltf");
	texHandle_ = TextureManager::Load("resources/uvChecker.png");
	anim_->SetTexHandle(texHandle_);
	/*postProcess_ = std::make_unique<PostProcess>();
	postProcess_->SetEffect(GaussianBlur);
	postProcess_->Initialize();
	texHandle_ = TextureManager::Load("resources/taiyou.jpg");
	sprite_.reset(Sprite::Create(texHandle_));*/

}

void GameScene::Update()
{
	animationTime_ += 1.0f / 60.0f;
	animationTime_ = fmod(animationTime_, 1.0f);
	//anim_->ApplyAnimation(animationTime_);
	//anim_->PlayAnimation();
	ImGui::Begin("camera");
	ImGui::DragFloat3("rotate", &camera_.rotate.x, 0.1f, -100.0f, 100.0f);
	ImGui::DragFloat3("trans", &camera_.translate.x, 0.1f, -100.0f, 100.0f);
	ImGui::DragFloat3("scale", &camera_.scale.x, 0.1f, -100.0f, 100.0f);
	ImGui::End();
	camera_.UpdateMatrix();
	worldTransform_.UpdateMatrix();
}

void GameScene::Draw()
{
	anim_->Draw(worldTransform_, camera_);
	//postProcess_->Draw();
}

void GameScene::PostProcessDraw()
{
	/*postProcess_->PreDraw();

	sprite_->Draw();

	postProcess_->PostDraw();*/
}
