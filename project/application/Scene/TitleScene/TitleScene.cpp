#include "TitleScene.h"

TitleScene::TitleScene()
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize()
{
	ModelResources::GetInstance()->LoadModel(); // 使うモデルをロードしておく
	texHandleTitle_ = TextureManager::Load("resources/title.png");
	spriteTitle_.reset(Sprite::Create(texHandleTitle_));

	texHandlePushA_ = TextureManager::Load("resources/pushA.png");
	spritePushA_.reset(Sprite::Create(texHandlePushA_,{330.0f,450.0f}));
}

void TitleScene::Update()
{
	// Aボタン押したらシーン遷移
	if (Input::GetInstance()->PressedButton(XINPUT_GAMEPAD_A)) {
		GameManager::GetInstance()->ChangeScene("GAME");
	}
}

void TitleScene::Draw()
{
	spriteTitle_->Draw();
	spritePushA_->Draw();
}
