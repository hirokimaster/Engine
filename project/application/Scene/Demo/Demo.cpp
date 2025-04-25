/**
* @file Demo.cpp
* @brief デモシーン
* @author 仁平 琉乃
*/

#include "Demo.h"

Demo::Demo()
{
}

Demo::~Demo()
{
}

void Demo::Initialize()
{
	camera_.Initialize();
	TextureManager::Load("resources/TempTexture/uvChecker.png");
	ModelManager::LoadObjModel("Player/cube.obj");
	objectManager_ = std::make_unique<ObjectManager>();
	object_ = objectManager_->CreateInstance("Player/cube.obj", TextureManager::GetTexHandle("TempTexture/uvChecker.png"));
	object_.lock()->worldTransform.scale = { 5.0f,5.0f,5.0f };

	object2_ = objectManager_->CreateInstance("Player/cube.obj", TextureManager::GetTexHandle("TempTexture/uvChecker.png"));
	object2_.lock()->worldTransform.scale = { 5.0f,5.0f,5.0f };
	object2_.lock()->worldTransform.translate = { 15.0f,0,0 };


}

void Demo::Update()
{

	camera_.UpdateMatrix();	
	objectManager_->Update();

	if (Input::GetInstance()->PressedKey(DIK_A)) {
		object_.lock()->isAlive = false;
	}

	if (Input::GetInstance()->PressedKey(DIK_C)) {
		object_.lock()->isAlive = true;
	}

	if (Input::GetInstance()->PressedKey(DIK_D)) {
		object2_.lock()->isAlive = true;
	}

	if (Input::GetInstance()->PressedKey(DIK_B)) {
		object2_.reset();
	}

	
}

void Demo::Draw()
{
	objectManager_->Draw(camera_);
}

void Demo::PostProcessDraw()
{
}