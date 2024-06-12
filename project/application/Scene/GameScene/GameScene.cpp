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
	postProcess_->SetEffect(DepthOutline);
	GameManager::GetInstance()->SetPostProcess(postProcess_.get());

	ModelManager::LoadObjModel("terrain.obj");
	texHandle_ = TextureManager::Load("resources/grass.png");

	object_ = std::make_unique<Object3DPlacer>();
	object_->Initialize();
	object_->SetModel("terrain.obj");
	object_->SetTexHandle(texHandle_);
	
	worldTransform_.Initialize();
	camera_.Initialize();
	
}

void GameScene::Update()
{
	ImGui::Begin("camera");
	ImGui::SliderAngle("rotateX", &camera_.rotate.x, 1.0f);
	ImGui::SliderAngle("rotateY", &camera_.rotate.y, 1.0f);
	ImGui::SliderAngle("rotateZ", &camera_.rotate.z, 1.0f);
	ImGui::DragFloat3("trans", &camera_.translate.x, 0.1f, -100.0f, 100.0f);
	ImGui::DragFloat3("scale", &camera_.scale.x, 0.1f, -100.0f, 100.0f);
	ImGui::End();

	camera_.UpdateMatrix();
	postProcess_->SetCamera(camera_);
	worldTransform_.UpdateMatrix();
}

void GameScene::Draw()
{
	postProcess_->Draw();	
}

void GameScene::PostProcessDraw()
{
	postProcess_->PreDraw();

	object_->Draw(worldTransform_, camera_);

	postProcess_->PostDraw();
}
