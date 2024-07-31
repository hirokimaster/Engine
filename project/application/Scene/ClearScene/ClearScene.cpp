#include "ClearScene.h"

ClearScene::ClearScene()
{
}

ClearScene::~ClearScene()
{
}

void ClearScene::Initialize()
{
	texHandleClear_ = TextureManager::Load("resources/clear.png");
	spriteClear_.reset(Sprite::Create(texHandleClear_));

	texHandlePushA_ = TextureManager::Load("resources/pushA.png");
	spritePushA_.reset(Sprite::Create(texHandlePushA_, { 330.0f,450.0f }));
}

void ClearScene::Update()
{
	// Aボタン押したらシーン遷移
	if (Input::GetInstance()->PressedButton(XINPUT_GAMEPAD_A)) {
		GameManager::GetInstance()->ChangeScene("TITLE");
	}
}

void ClearScene::Draw()
{
	spriteClear_->Draw();
	spritePushA_->Draw();
}
