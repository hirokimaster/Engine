/**
* @file TitleScene.cpp
* @brief タイトルシーン
* @author 仁平 琉乃
*/

#include "TitleScene.h"

TitleScene::TitleScene()
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize()
{
	// ポストエフェクト初期化
	postEffect_ = std::make_unique<PostEffect>();
	postEffect_->Initialize();

	LoadTextureFile(); // texture読み込み
	ModelResources::GetInstance()->LoadModel(); // 使うモデルをロードしておく

	titleSprite_ = std::make_unique<TitleSprite>();
	titleSprite_->Initialize();

	// 自機モデル
	objectPlayer_ = std::make_unique<Object3dPlacer>();
	objectPlayer_->Initialize();
	objectPlayer_->SetModel("Player/player.obj");
	objectPlayer_->SetTexHandle(TextureManager::GetTexHandle("TempTexture/white2.png"));
	
	// カメラ
	cameraManager_ = CameraManager::GetInstance();
	cameraManager_->Initialize();
	Vector3 offset = { -12.0f,0.5f,13.0f };
	Vector3 cameraRotate = { 0,std::numbers::pi_v<float> *3.0f / 4.0f ,0 };
	cameraManager_->SetCameraRotate(cameraRotate);
	cameraManager_->GetFollowCamera()->SetOffset(offset);
	cameraManager_->GetFollowCamera()->SetTarget(&objectPlayer_->GetWorldTransform());

	// シーン遷移用
	transition_ = std::make_unique<FadeOut>();
	transition_->Initialize();
	GameManager::GetInstance()->SetSceneTransition(transition_.get());
	isTransition_ = false;

	// 天球
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize();

	// particle
	particleManager_ = ParticleManager::GetInstance();
	particleManager_->Initialize();

	// particle
	engineParticle_ = std::make_unique<EngineParticle>();
	engineParticle_->Initialize();
}

void TitleScene::Update()
{
	// spriteの更新
	titleSprite_->Update();

	// Aボタンが押されたらシーン遷移処理を開始する
	if (Input::GetInstance()->PressedButton(XINPUT_GAMEPAD_A)) {
		if (!isTransition_) {// シーン遷移がまだ始まっていない場合のみ
			isTransition_ = true;
			transition_ = std::make_unique<FadeIn>();
			transition_->Initialize();
			GameManager::GetInstance()->SetSceneTransition(transition_.get());
			GameManager::GetInstance()->ChangeScene("GAME");
		}
	}

	objectPlayer_->Update();

	// particle
	if (objectPlayer_) {
		engineParticle_->Update(objectPlayer_->GetWorldTransform().rotate, objectPlayer_->GetWorldTransform().translate);
	}


	// カメラ
	cameraManager_->Update();
	
	// 天球
	skydome_->Update();

	// パーティクルマネージャ更新
	particleManager_->Update();
}

void TitleScene::Draw()
{
	postEffect_->GetPostProcess()->Draw();
}

void TitleScene::PostProcessDraw()
{
	postEffect_->GetPostProcess()->PreDraw();

	skydome_->Draw(cameraManager_->GetCamera());

	objectPlayer_->Draw(cameraManager_->GetCamera());

	titleSprite_->Draw();

	particleManager_->Draw(cameraManager_->GetCamera());

	postEffect_->GetPostProcess()->PostDraw();
}

void TitleScene::LoadTextureFile()
{
	TextureManager::Load("resources/TempTexture/noise0.png");
	TextureManager::Load("resources/TempTexture/white2.png");
	TextureManager::Load("resources/Scene/title.png");
	TextureManager::Load("resources/Scene/title.png");
	TextureManager::Load("resources/TempTexture/white.png");
	TextureManager::Load("resources/UI/A.png");
}
