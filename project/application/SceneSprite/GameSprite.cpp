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
	spriteSelect_ = nullptr;
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
		spriteSelect_->Draw();
	}
}

void GameSprite::Continue()
{
	// 作ってなかったら
	if (!spriteYes_ && !spriteNo_ && !spriteContinue_) {
		spriteYes_.reset(Sprite::Create(TextureManager::GetTexHandle("UI/yes.png"), spriteYesPosition_, texColor_));
		spriteYes_->SetAnchorPoint(Vector2(0.5f, 0.5f));
		spriteNo_.reset(Sprite::Create(TextureManager::GetTexHandle("UI/no.png"), spriteNoPosition_, texColor_));
		spriteNo_->SetAnchorPoint(Vector2(0.5f, 0.5f));
		spriteContinue_.reset(Sprite::Create(TextureManager::GetTexHandle("UI/continue.png"), spriteContinuePosition_, texColor_));
		spriteContinue_->SetAnchorPoint(Vector2(0.5f, 0.5f));
		spriteSelect_.reset(Sprite::Create(TextureManager::GetTexHandle("UI/select.png"), spriteSelectPosition_, texColor_));
		spriteSelect_->SetAnchorPoint(Vector2(0.5f, 0.5f));
	}
	
	// だんだん見えるようにする
	spriteContinue_->SetColor(texColor_);
	spriteYes_->SetColor(texColor_);
	spriteNo_->SetColor(texColor_);
	spriteSelect_->SetColor(texColor_);
	texColor_.w += 0.025f;

	// 1を越えたら固定する
	if (texColor_.w >= 1.0f) {
		spriteYes_->SetColor(texColor_);
		spriteNo_->SetColor(texColor_);
		spriteSelect_->SetColor(texColor_);
		texColor_.w = 1.0f;
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
		spriteYes_->SetScale({ scaleValue, scaleValue });
		spriteNo_->SetScale({ 1.0f, 1.0f });
	}
	else if (selectNo_ == 1) {
		spriteNo_->SetScale({ scaleValue, scaleValue });
		spriteYes_->SetScale({ 1.0f, 1.0f });
	}
	else {
		spriteYes_->SetScale({ 1.0f, 1.0f });
		spriteNo_->SetScale({ 1.0f, 1.0f });
	}
	// コンティニューのスケール変える
	spriteContinue_->SetScale({ scaleValue, scaleValue });

	ContinueSelect(); // 選択
	
}

void GameSprite::ContinueSelect()
{
	// コンティニューするかどうかの選択
	if (Input::GetInstance()->PressedButton(XINPUT_GAMEPAD_DPAD_LEFT) && selectNo_ == 1) {
		selectNo_ -= 1;
		spriteSelectPosition_ = { 380.0f,450.0f };
		spriteSelect_->SetPosition(spriteSelectPosition_);
	}
	else if (Input::GetInstance()->PressedButton(XINPUT_GAMEPAD_DPAD_RIGHT) && selectNo_ == 0) {
		selectNo_ += 1;
		spriteSelectPosition_ = { 680.0f,450.0f };
		spriteSelect_->SetPosition(spriteSelectPosition_);
	}


	// コンティニューするならリスタートやめるならタイトル
	if (texColor_.w >= 1.0f && !isTransitionTitle_) {
		if (Input::GetInstance()->PressedButton(XINPUT_GAMEPAD_A) && selectNo_ >= 1) {
			isTransitionTitle_ = true;
			transition_ = std::make_unique<FadeIn>();
			transition_->Initialize();
			GameManager::GetInstance()->SetSceneTransition(transition_.get());
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
	variables->AddItem(groupName, "spriteSelectPosition", spriteSelectPosition_);
}

void GameSprite::ApplyAdjustmentVariables()
{
	AdjustmentVariables* variables = AdjustmentVariables::GetInstance();
	const char* groupName = "GameSprite";
	spriteYesPosition_ = variables->GetValue<Vector2>(groupName, "spriteYesPosition");
	spriteNoPosition_ = variables->GetValue<Vector2>(groupName, "spriteNoPosition");
	spriteContinuePosition_ = variables->GetValue<Vector2>(groupName, "spriteContinuePosition");
	spriteSelectPosition_ = variables->GetValue<Vector2>(groupName, "spriteSelectPosition");
}


