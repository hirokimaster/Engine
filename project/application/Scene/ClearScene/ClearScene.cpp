/**
* @file ClearScene.cpp
* @brief クリアシーン
* @author 仁平 琉乃
*/

#include "ClearScene.h"

ClearScene::ClearScene()
{
}

ClearScene::~ClearScene()
{
}

void ClearScene::Initialize()
{
	isTransition_ = true;
	texHandleClear_ = TextureManager::Load("resources/Scene/clear.png");
	spriteClear_.reset(Sprite::Create(texHandleClear_, { 640.0f,260.0f }));
	spriteClear_->SetAnchorPoint({ 0.5f,0.5f });

	texHandlePushA_ = TextureManager::Load("resources/UI/A.png");
	spritePushA_.reset(Sprite::Create(texHandlePushA_, { 620.0f,500.0f }));

	// fade用のsprite
	texHandleWhite_ = TextureManager::Load("resources/TempTexture/white2.png");
	spriteColor_ = { 1.0f,1.0f,1.0f,1.0f };
	spriteFade_.reset(Sprite::Create(texHandleWhite_));
	spriteFade_->SetColor(spriteColor_);

	// playerのオブジェクト
	texHandlePlayer_ = TextureManager::Load("resources/TempTexture/white.png");
	objectPlayer_ = std::make_unique<Object3DPlacer>();
	objectPlayer_->Initialize();
	objectPlayer_->SetModel("Player/Jet.obj");
	objectPlayer_->SetTexHandle(texHandleWhite_);
	worldTransform_.Initialize();
	objectPlayer_->SetWorldTransform(worldTransform_);
	worldTransform_.translate.y = -5.0f;
	worldTransform_.translate.z = -50.0f;
	worldTransform_.translate.x = 8.0f;

	// 天球
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize();

	camera_.Initialize();
	camera_.translate.z = -30.0f;

	sceneTransition_ = SceneTransition::GetInstance();
}

void ClearScene::Update()
{

	camera_.UpdateMatrix();

	++animationTimer_;

	// Aボタン押したらシーン遷移
	if (Input::GetInstance()->PressedButton(XINPUT_GAMEPAD_A)) {
		sceneTransition_->SetPreScene(PreScene::Clear);
		sceneTransition_->FadeIn("TITLE");
	}

	// spriteClearのアニメーション用のやつ
	const float scaleSpeed = 0.8f;
	const float scaleRange = 0.05f;
	// アニメーションする
	scaleTimer_ += scaleSpeed * 1.0f / 60.0f;
	float scaleValue = 1.0f + scaleRange * std::sin(scaleTimer_);
	spriteClear_->SetScale({ scaleValue,scaleValue,scaleValue });

	// このシーンに来た時
	if (isTransition_) {
		sceneTransition_->FadeOut();
	}
	
	// 自機の移動
	const float moveSpeed = 1.2f;
	const float moveLimitZ = 300.0f;
	const float startPosZ = -50.0f;
	const float startPosX = 7.0f;
	worldTransform_.translate.z += moveSpeed;
	// 移動限界に達したら初期位置に戻す
	if (worldTransform_.translate.z >= moveLimitZ) {
		worldTransform_.translate.z = startPosZ;
		worldTransform_.translate.x = startPosX;
	}
	worldTransform_.UpdateMatrix();

	// 天球
	skydome_->Update();


}


void ClearScene::Draw()
{	

	skydome_->Draw(camera_);

	objectPlayer_->Draw(camera_);

	spriteClear_->Draw();

	if (animationTimer_ % 40 >= 20) {
		spritePushA_->Draw();
	}

	spriteFade_->Draw();

}
