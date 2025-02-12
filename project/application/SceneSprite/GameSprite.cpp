#include "GameSprite.h"
/**
* @file GameSprite.cpp
* @brief ゲームシーンのスプライト
* @author 仁平 琉乃
*/

void GameSprite::Initialize()
{
	spriteAttack_.reset(Sprite::Create(TextureManager::GetTexHandle("UI/RB.png"), { 1000.0f , 500.0f }));
	spriteAttack_->SetScale({ 2.0f,2.0f,2.0f });
	spriteMove_.reset(Sprite::Create(TextureManager::GetTexHandle("UI/L.png"), { 240.0f,500.0f }));
	spriteMove_->SetScale({ 2.0f,2.0f,2.0f });
}

void GameSprite::Draw()
{
	spriteAttack_->Draw();
	spriteMove_->Draw();
}

void GameSprite::Continue()
{
}
