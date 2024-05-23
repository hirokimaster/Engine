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
	/*worldTransform_.Initialize();
	worldTransform_.translate = { 2.0f,-0.5f,0 };
	worldTransform_.rotate.y = std::numbers::pi_v<float>;

	worldTransform_3.Initialize();
	worldTransform_3.translate = { -2.0f,-0.5f,0 };
	worldTransform_3.rotate.y = std::numbers::pi_v<float>;

	worldTransform_2.Initialize();
	worldTransform_2.translate = { 0,-0.5f,0 };
	worldTransform_2.rotate.y = std::numbers::pi_v<float>;
	camera_.Initialize();
	camera_.translate = { 0,0,-10.0f };

	ModelManager::LoadAnimationModel("simpleSkin.gltf");
	ModelManager::LoadAnimationModel("sneakWalk.gltf");
	ModelManager::LoadAnimationModel("Walk.gltf");

	anim_ = std::make_unique<Object3DPlacer>();
	anim_->Initialize();
	anim_->SetAnimModel("sneakWalk.gltf");
	anim_2 = std::make_unique<Object3DPlacer>();
	anim_2->Initialize();
	anim_2->SetAnimModel("Walk.gltf");
	anim_3 = std::make_unique<Object3DPlacer>();
	anim_3->Initialize();
	anim_3->SetAnimModel("simpleSkin.gltf");

	texHandle_ = TextureManager::Load("resources/uvChecker.png");
	anim_->SetTexHandle(texHandle_);

	anim_2->SetTexHandle(texHandle_);

	anim_3->SetTexHandle(texHandle_);*/

	/*postProcess_ = std::make_unique<PostProcess>();
	postProcess_->SetEffect(GaussianBlur);
	postProcess_->Initialize();
	texHandle_ = TextureManager::Load("resources/taiyou.jpg");
	sprite_.reset(Sprite::Create(texHandle_));*/

	camera_.Initialize();
	transform_.Initialize();

	texHandle_ = TextureManager::Load("resources/rostock_laage_airport_4k.dds");
	skyBox_ = std::make_unique<SkyBox>();
	skyBox_->Initialize();
	skyBox_->SetTexHandle(texHandle_);

}

void GameScene::Update()
{
    /*animationTime_ += 1.0f / 60.0f;
	animationTime_2 += 1.0f / 60.0f;
	animationTime_ = fmod(animationTime_, 1.0f);
	animationTime_2 = fmod(animationTime_2, 6.0f);*/

	ImGui::Begin("camera");
	ImGui::SliderAngle("rotateX", &camera_.rotate.x, 1.0f);
	ImGui::SliderAngle("rotateY", &camera_.rotate.y, 1.0f);
	ImGui::SliderAngle("rotateZ", &camera_.rotate.z, 1.0f);
	ImGui::DragFloat3("trans", &camera_.translate.x, 0.1f, -100.0f, 100.0f);
	ImGui::DragFloat3("scale", &camera_.scale.x, 0.1f, -100.0f, 100.0f);
	ImGui::End();

	/*
	ImGui::Begin("transform");
	ImGui::SliderAngle("rotateX", &worldTransform_.rotate.x,1.0f);
	ImGui::SliderAngle("rotateY", &worldTransform_.rotate.y, 1.0f);
	ImGui::SliderAngle("rotateZ", &worldTransform_.rotate.z, 1.0f);
	ImGui::DragFloat3("trans", &worldTransform_.translate.x, 0.1f, -100.0f, 100.0f);
	ImGui::DragFloat3("scale", &worldTransform_.scale.x, 0.1f, -100.0f, 100.0f);
	ImGui::End();

	ImGui::Begin("transform2");
	ImGui::SliderAngle("rotateX", &worldTransform_2.rotate.x,1.0f);
	ImGui::SliderAngle("rotateY", &worldTransform_2.rotate.y, 1.0f);
	ImGui::SliderAngle("rotateZ", &worldTransform_2.rotate.z, 1.0f);
	ImGui::DragFloat3("trans", &worldTransform_2.translate.x, 0.1f, -100.0f, 100.0f);
	ImGui::DragFloat3("scale", &worldTransform_2.scale.x, 0.1f, -100.0f, 100.0f);
	ImGui::End();*/

	/*camera_.UpdateMatrix();
	worldTransform_.UpdateMatrix();
	worldTransform_2.UpdateMatrix();
	worldTransform_3.UpdateMatrix();*/

	transform_.UpdateMatrix();
	camera_.UpdateMatrix();
}

void GameScene::Draw()
{
	/*anim_->SetAnimationTime(animationTime_);
	anim_->Draw(worldTransform_, camera_, true);
	anim_2->SetAnimationTime(animationTime_);
	anim_2->Draw(worldTransform_3, camera_, true);
	anim_3->SetAnimationTime(animationTime_2);
	anim_3->Draw(worldTransform_2, camera_, true);*/
	//postProcess_->Draw();

	skyBox_->Draw(transform_, camera_);
}

void GameScene::PostProcessDraw()
{
	/*postProcess_->PreDraw();

	sprite_->Draw();

	postProcess_->PostDraw();*/
}
