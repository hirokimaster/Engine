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
	postProcess_ = std::make_unique<PostProcess>();
	postProcess_->Initialize();
	postProcess_->SetEffect(PostEffectType::Dissolve);
	param_.threshold = 0.0f;
	postProcess_->SetDissolveParam(param_);

	LoadTextureFile(); // texture読み込み

	// ディゾルブ用
	spriteWhite_.reset(Sprite::Create(TextureManager::GetTexHandle("TempTexture/white2.png")));
	postProcess_->SetMaskTexture(TextureManager::GetTexHandle("TempTexture/noise0.png"));

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
	isTransition_ = false;
	sceneTransition_ = SceneTransition::GetInstance();
	sceneTransition_->Initialize();

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
		if (!isTransition_) {  // シーン遷移がまだ始まっていない場合のみ
			isTransition_ = true;
		}
	}

	// クリア、ゲームシーンから戻ってきたとき
	if (sceneTransition_->GetPreScene() == PreScene::Clear ||
		sceneTransition_->GetPreScene() == PreScene::Game) {
		sceneTransition_->FadeOut();
	}

	// シーン遷移
	if (isTransition_) {
		sceneTransition_->SetPreScene(PreScene::Title);
		sceneTransition_->FadeIn("GAME");
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

	spriteWhite_->Draw();

	postProcess_->Draw();

}

void TitleScene::PostProcessDraw()
{
	postProcess_->PreDraw();

	skydome_->Draw(followCamera_->GetCamera());

	objectPlayer_->Draw(followCamera_->GetCamera());

	titleSprite_->Draw();

	sceneTransition_->Draw();

	postProcess_->PostDraw();

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
