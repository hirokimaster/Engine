/**
* @file ClearScene.cpp
* @brief クリアシーン
* @author 仁平 琉乃
*/

#include "ClearScene.h"

ClearScene::ClearScene()
{
}

ClearScene::~ClearScene()
{
}

void ClearScene::Initialize()
{
	// 天球
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize();

	// sprite
	sprite_ = std::make_unique<ClearSprite>();
	sprite_->Initialize();

	cameraManager_ = CameraManager::GetInstance();
	cameraManager_->Initialize();
	cameraManager_->SetCameraTranslate({ 0,-30.0f,0 });

	// シーン遷移
	isTransition_ = false;
	transition_ = std::make_unique<FadeOut>();
	transition_->Initialize();
	GameManager::GetInstance()->SetSceneTransition(transition_.get());
}

void ClearScene::Update()
{

	cameraManager_->Update();

	// Aボタン押したらシーン遷移
	if (Input::GetInstance()->PressedButton(XINPUT_GAMEPAD_A) && !isTransition_) {
		isTransition_ = true;
		transition_ = std::make_unique<FadeIn>();
		transition_->Initialize();
		GameManager::GetInstance()->SetSceneTransition(transition_.get());
		GameManager::GetInstance()->ChangeScene("TITLE");
	}

	// sprite
	sprite_->Update();

	// 天球
	skydome_->Update();
}


void ClearScene::Draw()
{	

	skydome_->Draw(cameraManager_->GetCamera());

	sprite_->Draw();
}

void ClearScene::PostProcessDraw()
{

}
