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
	ModelResources::GetInstance()->LoadModel();
	texHandle_ = TextureManager::GetInstance()->Load("resources/TempTexture/circle.png");
	particleManager_ = ParticleManager::GetInstance();

	particleManager_->StartEditor("demoParticle");
	particleManager_->CreateParticle("demoParticle", "Player/plane.obj", texHandle_);
}

void Demo::Update()
{
	particleManager_->ApplyParticleInfo("demoParticle");
	particleManager_->Update("demoParticle");
	camera_.UpdateMatrix();		
}

void Demo::Draw()
{
	particleManager_->Draw("demoParticle", camera_);
}

void Demo::PostProcessDraw()
{
}