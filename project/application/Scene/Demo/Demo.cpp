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
	// particle
	particleManager_ = ParticleManager::GetInstance();
	particleManager_->Initialize();

	// particleManagerから取ってくる
	TextureManager::Load("resources/TempTexture/white.png");
	rightEngine_ = particleManager_->GetParticle("laserParticle", "TempTexture/white.png");
	rightEngine_->SetLifeTime(60000.0f);
	
}

void Demo::Update()
{
	camera_.UpdateMatrix();

	// パーティクルのパラメーターを設定
	rightEngine_->SetParticleParam(particleManager_->GetParam("laserParticle"));
	// アクティブにする
	rightEngine_->SetIsActive(true);
	
	particleManager_->Update();
}

void Demo::Draw()
{
	particleManager_->Draw(camera_);
}

void Demo::PostProcessDraw()
{
}