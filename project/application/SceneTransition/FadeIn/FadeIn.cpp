/**
* @file FadeIn.cpp
* @brief フェードイン
* @author 仁平 琉乃
*/

#include "FadeIn.h"

void FadeIn::Initialize()
{
	spriteWhite_.reset(Sprite::Create(TextureManager::GetTexHandle("TempTexture/white2.png"), { 0.0f,0.0f }, { 1.0f,1.0f,1.0f,0.0f }));
	alpha_ = 0.0f;
}

void FadeIn::Start(std::function<void()> call)
{
	isActive_ = true;
	onComplete_ = call;
}

void FadeIn::Update()
{
	// 開始してなかったら早期リターン
	if (!isActive_)return;

	const float speed = 0.02f;
	Vector4 color = { 1.0f,1.0f,1.0f,alpha_ };
	spriteWhite_->SetColor(color);
	alpha_ += speed;
	if (alpha_ >= 1.0f) {
		alpha_ = 1.0f;
		if (onComplete_)onComplete_();
	}
}

void FadeIn::Draw()
{
	// 開始してなかったら早期リターン
	if (!isActive_)return;
	spriteWhite_->Draw();
}
