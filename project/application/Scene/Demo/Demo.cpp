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
	ModelResources::GetInstance()->LoadModel(); // 使うモデルをロードしておく

	// 自機モデル
	texHandlePlayer_ = TextureManager::Load("resources/TempTexture/white.png");
	objectPlayer_ = std::make_unique<Object3DPlacer>();
	objectPlayer_->Initialize();
	objectPlayer_->SetModel("Player/Jet.obj");
	objectPlayer_->SetTexHandle(texHandlePlayer_);
	worldTransform_.Initialize();
	objectPlayer_->SetWorldTransform(worldTransform_);
	particle_ = std::make_unique<PlayerParticle>();
	particle_->Initialize();

	camera_.Initialize();
	camera_.translate.z = -20.0f;

}

void Demo::Update()
{
	worldTransform_.UpdateMatrix();
	particle_->SetPosition(worldTransform_.translate + particleOffset_);
	particle_->SetAreaMax({ worldTransform_.translate.x + 0.1f, worldTransform_.translate.y + 0.1f,worldTransform_.translate.z - 0.5f });
	particle_->SetAreaMin({ worldTransform_.translate.x - 0.1f, worldTransform_.translate.y - 0.1f,worldTransform_.translate.z - 0.7f });
	particle_->Update();

	camera_.UpdateMatrix();
}

void Demo::Draw()
{
	objectPlayer_->Draw(camera_);
	particle_->Draw(camera_);
}

void Demo::PostProcessDraw()
{
}
