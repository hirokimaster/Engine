/**
* @file TitleSprite.cpp
* @brief タイトルシーンで表示するスプライトの生成
* @author 仁平 琉乃
*/

#include "TitleSprite.h"

void TitleSprite::Initialize()
{
	// タイトルのスプライト
	spriteTitle_.reset(Sprite::Create(TextureManager::GetTexHandle("Scene/title.png"), { 650.0f,170.0f }));
	spriteTitle_->SetAnchorPoint({ 0.5f,0.5f });

	// Aボタンのスプライト
	spritePushA_.reset(Sprite::Create(TextureManager::GetTexHandle("UI/A.png"), { 620.0f,500.0f }));
}

void TitleSprite::Update()
{
	// spriteの点滅用のタイマーを足していく
	++animationTimer_;
}

void TitleSprite::Draw()
{
	spriteTitle_->Draw();

	if (animationTimer_ % 40 >= 20) {
		spritePushA_->Draw();
	}
}
