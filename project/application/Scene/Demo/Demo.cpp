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
	objectPlayer_->SetAnimModel("TempModel/player.gltf");
	objectPlayer_->SetTexHandle(texHandlePlayer_);
	worldTransform_.Initialize();
	objectPlayer_->SetWorldTransform(worldTransform_);

	camera_.Initialize();
	camera_.translate.z = -20.0f;

}

void Demo::Update()
{
	animTimer_ += 1.0f / 60.0f;
	objectPlayer_->SetAnimationTime(animTimer_);
	worldTransform_.UpdateMatrix();

	camera_.UpdateMatrix();
}

void Demo::Draw()
{
	objectPlayer_->Draw(camera_,true);
}

void Demo::PostProcessDraw()
{
}
