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
	line_ = std::make_unique<Line>();
	line_->Initialize({ -10.0f,0.0f,0.0f }, { 10.0f,0.0f,0.0f });
}

void Demo::Update()
{

	camera_.UpdateMatrix();		
}

void Demo::Draw()
{
	line_->Draw(camera_);
}

void Demo::PostProcessDraw()
{
}