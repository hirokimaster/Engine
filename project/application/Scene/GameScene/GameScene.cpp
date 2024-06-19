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
	postProcess_->SetEffect(Dissolve);
	/*param_.stepWidth = 0.001f;
	param_.sigma = 0.005f;
	param_.lightStrength = 1.0f;
	param_.bloomThreshold = 0.5f;*/
	/*param_.center = Vector2(0.5f, 0.5f);
	param_.blurWidth = 0.1f;*/
	param_.threshold = 0.5f;
	maskTex_ = TextureManager::Load("resources/noise0.png");
	postProcess_->SetMaskTexture(maskTex_);
	GameManager::GetInstance()->SetPostProcess(postProcess_.get());
	

	ModelManager::LoadObjModel("cube.obj");
	texHandle_ = TextureManager::Load("resources/uvChecker.png");
	

	object_ = std::make_unique<Object3DPlacer>();
	object_->Initialize();
	object_->SetModel("cube.obj");
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

	/*postProcess_->SetBloomParam(param_);

	ImGui::Begin("param");
	ImGui::DragFloat("stepWidth", &param_.stepWidth, 0.001f, 0.0f, 10.0f);
	ImGui::DragFloat("sigma", &param_.sigma, 0.001f, 0.0f, 10.0f);
	ImGui::DragFloat("lightStrength", &param_.lightStrength, 0.1f, 0.0f, 10.0f);
	ImGui::DragFloat("bloomThreshold", &param_.bloomThreshold, 0.01f, 0.0f, 10.0f);
	ImGui::End();*/

	//postProcess_->SetRadialParam(param_);
	postProcess_->SetDissolveParam(param_);

	/*ImGui::Begin("param");
	ImGui::DragFloat2("center", &param_.center.x, 0.1f, 0.0f, 10.0f);
	ImGui::DragFloat("blurWidth", &param_.blurWidth, 0.001f, 0.0f, 10.0f);
	ImGui::End();*/

	ImGui::Begin("param");
	ImGui::DragFloat("threshold", &param_.threshold, 0.01f, 0.0f, 10.0f);
	ImGui::End();

	camera_.UpdateMatrix();
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
