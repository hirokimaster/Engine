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

	ModelManager::GetInstance()->LoadObjModel("Player/ball.obj");
	// particleManagerから取ってくる
	TextureManager::Load("resources/TempTexture/white.png");
	rightEngine_ = particleManager_->GetParticle("explosion", "TempTexture/white.png");
	rightEngine_->SetModel("Player/ball.obj");
	rightEngine_->SetLifeTime(60000.0f);
	
}

void Demo::Update()
{
	camera_.UpdateMatrix();

	// パーティクルのパラメーターを設定
	rightEngine_->SetParticleParam(particleManager_->GetParam("explosion"));
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