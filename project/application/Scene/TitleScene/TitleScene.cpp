#include "TitleScene.h"

TitleScene::TitleScene()
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize()
{	
	texHandleTitle_ = TextureManager::Load("resources/Title.png");
	spriteTitle_.reset(Sprite::Create(texHandleTitle_));

}

void TitleScene::Update()
{
	if (Input::GetInstance()->PressedKey(DIK_A)) {
		GameManager::GetInstance()->ChangeScene("GAME");
	}
}

void TitleScene::Draw()
{
	spriteTitle_->Draw();
}
