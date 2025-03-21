/**
* @file FadeOut.cpp
* @brief フェードアウト
* @author 仁平 琉乃
*/

#include "FadeOut.h"

void FadeOut::Initialize()
{
	spriteWhite_.reset(Sprite::Create(TextureManager::GetTexHandle("TempTexture/white2.png"), { 0.0f,0.0f }, { 1.0f,1.0f,1.0f,0.0f }));
	alpha_ = 1.0f;
}

void FadeOut::Start(std::function<void()> call)
{
	isActive_ = true;
	onComplete_ = call;
}

void FadeOut::Update()
{
	// 開始してなかったら早期リターン
	if (!isActive_)return;
    
	const float speed = 0.02f;
	Vector4 color = { 1.0f,1.0f,1.0f,alpha_ };
	spriteWhite_->SetColor(color);
	alpha_ -= speed;
	if (alpha_ <= 0.0f) {
		alpha_ = 0.0f;
		isActive_ = false;
		if (onComplete_)onComplete_();
	}
}

void FadeOut::Draw()
{
	// 開始してなかったら早期リターン
	if (!isActive_)return;
	spriteWhite_->Draw();
}
