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
	TextureManager::Load("resources/Scene/clear.png");
	spriteClear_.reset(Sprite::Create(TextureManager::GetTexHandle("Scene/clear.png"), { 640.0f,260.0f }));
	spriteClear_->SetAnchorPoint({ 0.5f,0.5f });

	spritePushA_.reset(Sprite::Create(TextureManager::GetTexHandle("UI/A.png"), { 620.0f,500.0f }));

	// 天球
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize();

	camera_.Initialize();
	camera_.translate.z = -30.0f;

	// シーン遷移
	isTransition_ = false;
	transition_ = std::make_unique<FadeOut>();
	transition_->Initialize();
	GameManager::GetInstance()->SetSceneTransition(transition_.get());
}

void ClearScene::Update()
{

	camera_.UpdateMatrix();

	++animationTimer_;

	// Aボタン押したらシーン遷移
	if (Input::GetInstance()->PressedButton(XINPUT_GAMEPAD_A) && !isTransition_) {
		isTransition_ = true;
		transition_ = std::make_unique<FadeIn>();
		transition_->Initialize();
		GameManager::GetInstance()->SetSceneTransition(transition_.get());
		GameManager::GetInstance()->ChangeScene("TITLE");
	}

	// spriteClearのアニメーション用のやつ
	const float scaleSpeed = 0.8f;
	const float scaleRange = 0.05f;
	// アニメーションする
	scaleTimer_ += scaleSpeed * 1.0f / 60.0f;
	float scaleValue = 1.0f + scaleRange * std::sin(scaleTimer_);
	spriteClear_->SetScale({ scaleValue,scaleValue});

	// 天球
	skydome_->Update();
}


void ClearScene::Draw()
{	

	skydome_->Draw(camera_);

	spriteClear_->Draw();

	if (animationTimer_ % 40 >= 20) {
		spritePushA_->Draw();
	}
}

void ClearScene::PostProcessDraw()
{

}
