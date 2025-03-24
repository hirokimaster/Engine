#include "GameSprite.h"
#include "application/Scene/GameScene/GameScene.h"
/**
* @file GameSprite.cpp
* @brief ゲームシーンのスプライト
* @author 仁平 琉乃
*/

void GameSprite::Initialize()
{
	// ゲームオーバー用
	texColor_ = { 1.0f,1.0f,1.0f,0.0f };
	// 初期化
	spriteYes_ = nullptr;
	spriteNo_ = nullptr;
	spriteContinue_ = nullptr;
	// 調整項目追加
	AddAdjustmentVariables();
	// 調整項目適用
	ApplyAdjustmentVariables();
}

void GameSprite::Update()
{
	// playerが死んだら
	if (player_->GetIsDead()) {
		Continue();
	}
}

void GameSprite::Draw()
{
	// 作ってあったら表示する
	if (spriteYes_ && spriteNo_ && spriteContinue_) {
		spriteNo_->Draw();
		spriteYes_->Draw();
		spriteContinue_->Draw();
	}
}

void GameSprite::Continue()
{
	// 作ってなかったら
	if (!spriteYes_ && !spriteNo_ && !spriteContinue_) {
		spriteYes_.reset(Sprite::Create(TextureManager::GetTexHandle("UI/yes.png"), spriteYesPosition_, texColor_));
		spriteNo_.reset(Sprite::Create(TextureManager::GetTexHandle("UI/no.png"), spriteNoPosition_, texColor_));
		spriteContinue_.reset(Sprite::Create(TextureManager::GetTexHandle("UI/continue.png"), spriteContinuePosition_, texColor_));
	}
	
	spriteContinue_->SetColor(texColor_);
	spriteYes_->SetColor(texColor_);
	spriteNo_->SetColor(texColor_);
	texColor_.w += 0.05f;

	if (texColor_.w >= 2.0f) {
		spriteYes_->SetColor(texColor_);
		spriteNo_->SetColor(texColor_);
		texColor_.w = 2.0f;
	}

	// コンティニューするかどうかの選択
	if (Input::GetInstance()->PressedButton(XINPUT_GAMEPAD_DPAD_LEFT) && selectNo_ == 1) {
		selectNo_ -= 1;
	}
	else if (Input::GetInstance()->PressedButton(XINPUT_GAMEPAD_DPAD_RIGHT) && selectNo_ == 0) {
		selectNo_ += 1;
	}

	// UIのアニメーション用の変数
	static float scaleTimer = 0.0f;
	const float scaleSpeed = 2.0f;
	const float scaleRange = 0.2f;

	// spriteNo,Yesのアニメーション
	scaleTimer += scaleSpeed * 1.0f / 60.0f;
	float scaleValue = 1.0f + scaleRange * sin(scaleTimer);

	// 選択してるスプライトによって変える
	if (selectNo_ == 0) {
		spriteYes_->SetScale({ scaleValue, scaleValue});
		spriteNo_->SetScale({ 1.0f, 1.0f});
	}
	else if (selectNo_ == 1) {
		spriteNo_->SetScale({ scaleValue, scaleValue});
		spriteYes_->SetScale({ 1.0f, 1.0f});
	}
	else {
		spriteYes_->SetScale({ 1.0f, 1.0f});
		spriteNo_->SetScale({ 1.0f, 1.0f});
	}

	// コンティニューするならリスタートやめるならタイトル
	if (texColor_.w >= 2.0f) {
		if (Input::GetInstance()->PressedButton(XINPUT_GAMEPAD_A) && selectNo_ >= 1) {
			GameManager::GetInstance()->ChangeScene("TITLE");
		}

		if (Input::GetInstance()->PressedButton(XINPUT_GAMEPAD_A) && selectNo_ <= 0) {
			gameScene_->Initialize();
		}
	}
}

void GameSprite::AddAdjustmentVariables()
{
	AdjustmentVariables* variables = AdjustmentVariables::GetInstance();
	const char* groupName = "GameSprite";
	// グループを追加
	variables->CreateGroup(groupName);
	// アイテム追加
	variables->AddItem(groupName, "spriteYesPosition", spriteYesPosition_);
	variables->AddItem(groupName, "spriteNoPosition", spriteNoPosition_);
	variables->AddItem(groupName, "spriteContinuePosition", spriteContinuePosition_);
}

void GameSprite::ApplyAdjustmentVariables()
{
	AdjustmentVariables* variables = AdjustmentVariables::GetInstance();
	const char* groupName = "GameSprite";
	spriteYesPosition_ = variables->GetValue<Vector2>(groupName, "spriteYesPosition");
	spriteNoPosition_ = variables->GetValue<Vector2>(groupName, "spriteNoPosition");
	spriteContinuePosition_ = variables->GetValue<Vector2>(groupName, "spriteContinuePosition");
}


