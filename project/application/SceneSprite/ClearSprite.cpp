#include "ClearSprite.h"
/**
* @file ClaerSprite.cpp
* @brief クリアシーンのスプライト
* @author 仁平 琉乃
*/

void ClearSprite::Initialize()
{
	TextureManager::Load("resources/Scene/clear.png"); // texture読み込み

	// sprite生成
	spriteClear_.reset(Sprite::Create(TextureManager::GetTexHandle("Scene/clear.png"), { 640.0f,260.0f }));
	spriteClear_->SetAnchorPoint({ 0.5f,0.5f });

	spritePushA_.reset(Sprite::Create(TextureManager::GetTexHandle("UI/A.png"), { 620.0f,500.0f }));
}

void ClearSprite::Update()
{
	++animationTimer_; // Aボタン用

	// spriteClearのアニメーション用のやつ
	const float scaleSpeed = 0.8f;
	const float scaleRange = 0.05f;
	// アニメーションする
	scaleTimer_ += scaleSpeed * 1.0f / 60.0f;
	float scaleValue = 1.0f + scaleRange * std::sin(scaleTimer_);
	spriteClear_->SetScale({ scaleValue,scaleValue });
}

void ClearSprite::Draw()
{
	spriteClear_->Draw();

	if (animationTimer_ % 40 >= 20) {
		spritePushA_->Draw();
	}
}
