#include "ClearSprite.h"
/**
* @file ClaerSprite.cpp
* @brief クリアシーンのスプライト
* @author 仁平 琉乃
*/

void ClearSprite::Initialize()
{
	TextureManager::Load("resources/Scene/clear.png"); // texture読み込み

	// 調整項目追加
	AddAdjustmentVariables();
	// 調整項目適用
	ApplyAdjustmentVariables();
	// sprite生成
	spriteClear_.reset(Sprite::Create(TextureManager::GetTexHandle("Scene/clear.png"), spriteClearPosition_));
	spriteClear_->SetAnchorPoint({ 0.5f,0.5f });

	spritePushA_.reset(Sprite::Create(TextureManager::GetTexHandle("UI/A.png"), spritePushAPosition_));
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

void ClearSprite::AddAdjustmentVariables()
{
	AdjustmentVariables* variables = AdjustmentVariables::GetInstance();
	const char* groupName = "ClearSprite";
	// グループを追加
	variables->CreateGroup(groupName);
	// アイテム追加
	variables->AddItem(groupName, "spriteClearPosition", spriteClearPosition_);
	variables->AddItem(groupName, "spritePushAPosition", spritePushAPosition_);
}

void ClearSprite::ApplyAdjustmentVariables()
{
	AdjustmentVariables* variables = AdjustmentVariables::GetInstance();
	const char* groupName = "ClearSprite";
	spriteClearPosition_ = variables->GetValue<Vector2>(groupName, "spriteClearPosition");
	spritePushAPosition_ = variables->GetValue<Vector2>(groupName, "spritePushAPosition");
}
