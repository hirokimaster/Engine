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
	spriteWhite_.reset(Sprite::Create(texHandleWhite_));
	postProcess_->SetMaskTexture(texHandleMask_);

	ModelResources::GetInstance()->LoadModel(); // 使うモデルをロードしておく

	// タイトルのスプライト
	spriteTitle_.reset(Sprite::Create(texHandleTitle_, { 650.0f,170.0f }));
	spriteTitle_->SetAnchorPoint({ 0.5f,0.5f });

	texHandlePushA_ = TextureManager::Load("resources/UI/A.png");
	spritePushA_.reset(Sprite::Create(texHandlePushA_, { 620.0f,500.0f }));

	// 自機モデル
	objectPlayer_ = std::make_unique<Object3DPlacer>();
	objectPlayer_->Initialize();
	objectPlayer_->SetModel("Player/Jet.obj");
	objectPlayer_->SetTexHandle(texHandleWhite_);
	worldTransform_.Initialize();
	objectPlayer_->SetWorldTransform(worldTransform_);

	// カメラ
	camera_.Initialize();

	// 追従カメラ
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();
	Vector3 offset = { -12.0f,0.5f,13.0f };
	Vector3 cameraRotate = { 0,std::numbers::pi_v<float> *3.0f / 4.0f ,0 };
	followCamera_->SetOffset(offset);
	followCamera_->SetRotate(cameraRotate);
	followCamera_->SetTarget(&worldTransform_);

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
	// spriteの点滅用のタイマー
	++animationTimer_;

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
	Vector3 move = { 0,0,2.0f };
	worldTransform_.translate = worldTransform_.translate + move;
	worldTransform_.UpdateMatrix();

	// カメラ
	followCamera_->Update();
	camera_.matView = followCamera_->GetCamera().matView;
	camera_.matProjection = followCamera_->GetCamera().matProjection;
	camera_.TransferMatrix();

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

	skydome_->Draw(camera_);

	objectPlayer_->Draw(camera_);

	spriteTitle_->Draw();

	if (animationTimer_ % 40 >= 20) {
		spritePushA_->Draw();
	}

	sceneTransition_->Draw();

	postProcess_->PostDraw();

}

void TitleScene::LoadTextureFile()
{
	texHandleMask_ = TextureManager::Load("resources/TempTexture/noise0.png");
	texHandleWhite_ = TextureManager::Load("resources/TempTexture/white2.png");
	texHandleTitle_ = TextureManager::Load("resources/Scene/title.png");
	texHandlePlayer_ = TextureManager::Load("resources/TempTexture/white.png");
}
