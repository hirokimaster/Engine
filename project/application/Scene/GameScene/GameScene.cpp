#include "GameScene.h"
#include "engine/ModelManager/ModelManager.h"
#define CUSTOM_DEADZONE 12000

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

void GameScene::Initialize()
{
	camera_.Initialize();
	camera_.translate = { 0,0,-10.0f };

	ModelManager::LoadAnimationModel("Walk.gltf");
	ModelManager::LoadObjModel("cube.obj");

	anim_ = std::make_unique<Object3DPlacer>();
	anim_->Initialize();
	//anim_->SetModel("cube.obj");
	anim_->SetAnimModel("Walk.gltf");

	texHandle_ = TextureManager::Load("resources/uvChecker.png");
	anim_->SetTexHandle(texHandle_);

	worldTransform_.Initialize();
	worldTransform_.translate = { 0,-0.5f,0 };
	worldTransform_.rotate.y = std::numbers::pi_v<float>;

	/*camera_.Initialize();
	transform_.Initialize();
	transform_.scale = { 100.0f,100.0f,100.0f };
	worldTransform_.Initialize();
	worldTransform_.scale = { 1.0f,1.0f,1.0f };
	worldTransform_.translate = { 0,-0.5f,-50.0f };
	worldTransform_.rotate.y = std::numbers::pi_v<float>;

	texHandle2_ = TextureManager::Load("resources/white.png");
	ModelManager::LoadGLTFModel("Walk.gltf");

	object_ = std::make_unique<Object3DPlacer>();
	object_->Initialize();
	object_->SetModel("Walk.gltf");
	object_->SetTexHandle(texHandle2_);

	texHandle_ = TextureManager::Load("resources/rostock_laage_airport_4k.dds");
	skyBox_ = std::make_unique<SkyBox>();
	skyBox_->Initialize();
	skyBox_->SetTexHandle(texHandle_);

	light_ = std::make_unique<Lighting>();
	light_->Initialize(Environment);
	light_->SetEnvironmentTexture(texHandle_);
	material_.environmentCoefficient = 1.0f;
	material_.shininess = 10.0f;*/

}

void GameScene::Update()
{

	XINPUT_STATE joyState{};

	Vector3 move{ 0,0,0 };

	if (Input::GetInstance()->GetJoystickState(joyState)) {
		if ((joyState.Gamepad.sThumbLX < CUSTOM_DEADZONE && joyState.Gamepad.sThumbLX > -CUSTOM_DEADZONE)) {
			joyState.Gamepad.sThumbLX = 0;
			joyState.Gamepad.sThumbLY = 0;
			move.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * kMoveSpeed_;
		}
		else {
			move.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * kMoveSpeed_;
			animationTime_ += 1.0f / 60.0f;
			if ((float)joyState.Gamepad.sThumbLX < -CUSTOM_DEADZONE) {
				worldTransform_.rotate.y = -std::numbers::pi_v<float> / 2.0f;
			}
	
			if ((float)joyState.Gamepad.sThumbLX > CUSTOM_DEADZONE) {
				worldTransform_.rotate.y = std::numbers::pi_v<float> / 2.0f;
			}
		}
	}

	worldTransform_.translate = Add(worldTransform_.translate, move);

	worldTransform_.UpdateMatrix();

	animationTime_ = fmod(animationTime_, 1.0f);
#ifdef _DEBUG

	ImGui::Begin("camera");	d
	ImGui::SliderAngle("rotateX", &camera_.rotate.x, 1.0f);
	ImGui::SliderAngle("rotateY", &camera_.rotate.y, 1.0f);
	ImGui::SliderAngle("rotateZ", &camera_.rotate.z, 1.0f);
	ImGui::DragFloat3("trans", &camera_.translate.x, 0.1f, -100.0f, 100.0f);
	ImGui::DragFloat3("scale", &camera_.scale.x, 0.1f, -100.0f, 100.0f);
	ImGui::End();

	ImGui::Begin("transform");
	ImGui::SliderAngle("rotateX", &worldTransform_.rotate.x, 1.0f);
	ImGui::SliderAngle("rotateY", &worldTransform_.rotate.y, 1.0f);
	ImGui::SliderAngle("rotateZ", &worldTransform_.rotate.z, 1.0f);
	ImGui::DragFloat3("trans", &worldTransform_.translate.x, 0.1f, -100.0f, 100.0f);
	ImGui::DragFloat3("scale", &worldTransform_.scale.x, 0.1f, -100.0f, 100.0f);
	ImGui::End();

#endif

	/*object_->SetLight(light_.get());
	object_->SetMaterialProperty(material_);
	material_.enableLighting = true;
	material_.color = { 1.0f,1.0f,1.0f,1.0f };
	light_->SetCameraData(camera_.translate);

	ImGui::Begin("material");
	ImGui::DragFloat("shiness", &material_.shininess, 0.1f, -100.0f, 100.0f);
	ImGui::DragFloat("scale", &material_.environmentCoefficient, 0.1f, -100.0f, 100.0f);
	ImGui::End();*/

	/*
	
	ImGui::Begin("transform2");
	ImGui::SliderAngle("rotateX", &worldTransform_2.rotate.x,1.0f);
	ImGui::SliderAngle("rotateY", &worldTransform_2.rotate.y, 1.0f);
	ImGui::SliderAngle("rotateZ", &worldTransform_2.rotate.z, 1.0f);
	ImGui::DragFloat3("trans", &worldTransform_2.translate.x, 0.1f, -100.0f, 100.0f);
	ImGui::DragFloat3("scale", &worldTransform_2.scale.x, 0.1f, -100.0f, 100.0f);
	ImGui::End();*/

	/*camera_.UpdateMatrix();
	
	worldTransform_2.UpdateMatrix();
	worldTransform_3.UpdateMatrix();*/

	//worldTransform_.rotate.y += 0.03f;
	//transform_.UpdateMatrix();
	
	camera_.UpdateMatrix();
}

void GameScene::Draw()
{
	anim_->SetAnimationTime(animationTime_);
	anim_->Draw(worldTransform_, camera_, true);

	/*skyBox_->Draw(transform_, camera_);
	object_->Draw(worldTransform_, camera_);*/
}

void GameScene::PostProcessDraw()
{
}
