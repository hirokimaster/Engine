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
	ModelManager::GetInstance()->LoadObjModel("Player/plane.obj");
}

void Demo::Update()
{
	camera_.UpdateMatrix();
}

void Demo::Draw()
{
	ParticleEditor::GetInstance()->Draw(camera_);
}

void Demo::PostProcessDraw()
{
}