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
	worldTransform_.rotate.y = std::numbers::pi_v<float>;
	worldTransform_2.Initialize();
	worldTransform_2.rotate.y = std::numbers::pi_v<float>;
	camera_.Initialize();
	camera_.translate = { 0,0,-10.0f };

	ModelManager::LoadGLTFModel("walk.gltf");

	anim_ = std::make_unique<ModelAnimation>();
	anim_->Initialize("walk.gltf");
	texHandle_ = TextureManager::Load("resources/white.png");
	anim_->SetTexHandle(texHandle_);

	object_ = std::make_unique<Object3DPlacer>();
	object_->Initialize();
	object_->SetModel("walk.gltf");
	object_->SetTexHandle(texHandle_);
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
	ImGui::Begin("camera");
	ImGui::SliderAngle("rotateX", &camera_.rotate.x, 1.0f);
	ImGui::SliderAngle("rotateY", &camera_.rotate.y, 1.0f);
	ImGui::SliderAngle("rotateZ", &camera_.rotate.z, 1.0f);
	ImGui::DragFloat3("trans", &camera_.translate.x, 0.1f, -100.0f, 100.0f);
	ImGui::DragFloat3("scale", &camera_.scale.x, 0.1f, -100.0f, 100.0f);
	ImGui::End();

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
	ImGui::End();

	camera_.UpdateMatrix();
	worldTransform_.UpdateMatrix();
	worldTransform_2.UpdateMatrix();
}

void GameScene::Draw()
{
	anim_->SetAnimationTime(animationTime_);
	anim_->Draw(worldTransform_, camera_);
	object_->Draw(worldTransform_2, camera_);
	//postProcess_->Draw();
}

void GameScene::PostProcessDraw()
{
	/*postProcess_->PreDraw();

	sprite_->Draw();

	postProcess_->PostDraw();*/
}
