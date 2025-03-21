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
	objectPlayer_ = std::make_unique<Object3DPlacer>();
	objectPlayer_->Initialize();
	objectPlayer_->SetModel("Player/player.obj");
	objectPlayer_->SetTexHandle(TextureManager::GetTexHandle("TempTexture/white2.png"));
	
	// 追従カメラ
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();
	Vector3 offset = { -12.0f,0.5f,13.0f };
	Vector3 cameraRotate = { 0,std::numbers::pi_v<float> *3.0f / 4.0f ,0 };
	followCamera_->SetOffset(offset);
	followCamera_->SetRotate(cameraRotate);
	followCamera_->SetTarget(&objectPlayer_->GetWorldTransform());

	// シーン遷移用
	transition_ = std::make_unique<FadeOut>();
	transition_->Initialize();
	GameManager::GetInstance()->SetSceneTransition(transition_.get());
	isTransition_ = false;

	// 天球
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize();
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

	// 自機の移動
	Vector3 velocity = { 0,0,2.0f };
	Vector3 move{};
	move = objectPlayer_->GetWorldTransform().translate + velocity;
	objectPlayer_->SetPosition(move);
	objectPlayer_->Update();

	// カメラ
	followCamera_->Update();
	
	// 天球
	skydome_->Update();
}

void TitleScene::Draw()
{
	postEffect_->GetPostProcess()->Draw();
}

void TitleScene::PostProcessDraw()
{
	postEffect_->GetPostProcess()->PreDraw();

	skydome_->Draw(followCamera_->GetCamera());

	objectPlayer_->Draw(followCamera_->GetCamera());

	titleSprite_->Draw();

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
