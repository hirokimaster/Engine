#include "SceneTransition.h"

SceneTransition* SceneTransition::GetInstance()
{
	static SceneTransition instance;
	return &instance;
}

void SceneTransition::Initialize()
{
	spriteWhite_.reset(Sprite::Create(TextureManager::GetTexHandle("TempTexture/white2.png"), { 0.0f,0.0f }, { 1.0f,1.0f,1.0f,0.0f }));
}

void SceneTransition::FadeIn(const std::string& SceneName)
{
	Vector4 color = { 1.0f,1.0f,1.0f,alpha_ };
	spriteWhite_->SetColor(color);
	alpha_ += 0.02f;
	if (alpha_ >= 1.0f) {
		fadeState_ = true;
		GameManager::GetInstance()->ChangeScene(SceneName);
	}		   
}

void SceneTransition::FadeOut()
{
	Vector4 color = { 1.0f,1.0f,1.0f,alpha_ };
	spriteWhite_->SetColor(color);
	alpha_ -= 0.02f;
	if (alpha_ <= 0.0f && fadeState_) {
		alpha_ = 0.0f;
		fadeState_ = false;
	}
}

void SceneTransition::Draw()
{
	spriteWhite_->Draw();
}
