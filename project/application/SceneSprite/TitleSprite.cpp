/**
* @file TitleSprite.cpp
* @brief タイトルシーンで表示するスプライトの生成
* @author 仁平 琉乃
*/

#include "TitleSprite.h"

void TitleSprite::Initialize()
{
	// 調整項目追加
	AddAdjustmentVariables();
	// 調整項目適用
	ApplyAdjustmentVariables();
	// タイトルのスプライト
	spriteTitle_.reset(Sprite::Create(TextureManager::GetTexHandle("Scene/title.png"), spriteTitlePosition_));
	spriteTitle_->SetAnchorPoint({ 0.5f,0.5f });
	spriteTitle_->SetColor({ 0.3f,0.3f,0.3f,1.0f });

	// Aボタンのスプライト
	spritePushA_.reset(Sprite::Create(TextureManager::GetTexHandle("UI/A.png"), spritePushAPosition_));
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

void TitleSprite::AddAdjustmentVariables()
{
	AdjustmentVariables* variables = AdjustmentVariables::GetInstance();
	const char* groupName = "TitleSprite";
	// グループを追加
	variables->CreateGroup(groupName);
	// アイテム追加
	variables->AddItem(groupName, "spriteTitlePosition", spriteTitlePosition_);
	variables->AddItem(groupName, "spritePushAPosition", spritePushAPosition_);
}

void TitleSprite::ApplyAdjustmentVariables()
{
	AdjustmentVariables* variables = AdjustmentVariables::GetInstance();
	const char* groupName = "TitleSprite";
	spriteTitlePosition_ = variables->GetValue<Vector2>(groupName, "spriteTitlePosition");
	spritePushAPosition_ = variables->GetValue<Vector2>(groupName, "spritePushAPosition");
}
