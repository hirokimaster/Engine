/**
* @file Demo.cpp
* @brief デモシーン
* @author 仁平 琉乃
*/

#include "Demo.h"
#include "engine/ModelManager/ModelManager.h"

Demo::Demo()
{
}

Demo::~Demo()
{
}

void Demo::Initialize()
{
	camera_.Initialize();
	texHandle_ = TextureManager::GetInstance()->Load("resources/TempTexture/smoke.dds");
	sprite_.reset(Sprite::Create(texHandle_));

}

void Demo::Update()
{

	camera_.UpdateMatrix();		
}

void Demo::Draw()
{
	sprite_->Draw();
}

void Demo::PostProcessDraw()
{
}