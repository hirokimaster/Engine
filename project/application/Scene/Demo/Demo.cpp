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
	TextureManager::Load("resources/Player/engine.png");
	p_ = ParticleManager::GetInstance();
	p_->CreateParam("engine");
	p_->ApplyParam("engine");
	particle_ = p_->GetParticle("engine", "Player/engine.png");
}

void Demo::Update()
{
	p_->ApplyParam("engine");
	particle_->SetIsActive(true);
	particle_->SetParticleParam(p_->GetParam("engine"));
	particle_->Update();
	camera_.UpdateMatrix();		
}

void Demo::Draw()
{
	particle_->Draw(camera_);
}

void Demo::PostProcessDraw()
{
}