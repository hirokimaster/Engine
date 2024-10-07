#include "TitleScene.h"

TitleScene::TitleScene()
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize()
{

	postProcess_ = std::make_unique<PostProcess>();
	postProcess_->Initialize();
	postProcess_->SetEffect(PostEffectType::Dissolve);

	param_.threshold = 0.0f;
	postProcess_->SetDissolveParam(param_);
	texHandleMask_ = TextureManager::Load("resources/TempTexture/noise9.png");
	texHandleWhite_ = TextureManager::Load("resources/TempTexture/white2.png");
	postProcess_->SetMaskTexture(texHandleMask_);
	spriteWhite_.reset(Sprite::Create(texHandleWhite_));

	ModelResources::GetInstance()->LoadModel(); // 使うモデルをロードしておく

	texHandleTitle_ = TextureManager::Load("resources/Scene/title.png");
	spriteTitle_.reset(Sprite::Create(texHandleTitle_));

	texHandlePushA_ = TextureManager::Load("resources/UI/A.png");
	spritePushA_.reset(Sprite::Create(texHandlePushA_, { 620.0f,500.0f }));

	// 自機モデル
	texHandlePlayer_ = TextureManager::Load("resources/TempTexture/white.png");
	objectPlayer_ = std::make_unique<Object3DPlacer>();
	objectPlayer_->Initialize();
	objectPlayer_->SetModel("Player/Jet.obj");
	objectPlayer_->SetTexHandle(texHandleWhite_);
	worldTransform_.Initialize();
	objectPlayer_->SetWorldTransform(worldTransform_);
	particle_ = std::make_unique<PlayerParticle>();
	particle_->Initialize();

	// 天球
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize();

	// カメラ
	camera_.Initialize();

	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();
	Vector3 offset = { -12.0f,0,13.0f };
	Vector3 cameraRotate = { 0,std::numbers::pi_v<float> *3.0f / 4.0f ,0 };
	followCamera_->SetOffset(offset);
	followCamera_->SetRotate(cameraRotate);
	followCamera_->SetTarget(&worldTransform_);
}

void TitleScene::Update()
{
	++startATimer_;
	// Aボタン押したらシーン遷移
	/*if (Input::GetInstance()->PressedButton(XINPUT_GAMEPAD_A)) {
		GameManager::GetInstance()->ChangeScene("GAME");
	}*/

	// 自機
	Vector3 move = { 0,0,0.2f };
	worldTransform_.translate = worldTransform_.translate + move;
	worldTransform_.UpdateMatrix();
	Vector3 particleOffset = { 0,0.5f,-4.0f };
	particle_->SetPosition(worldTransform_.translate + particleOffset);
	particle_->SetAreaMax({ worldTransform_.translate.x + 0.1f, worldTransform_.translate.y + 0.1f,worldTransform_.translate.z - 0.5f });
	particle_->SetAreaMin({ worldTransform_.translate.x - 0.1f, worldTransform_.translate.y - 0.1f,worldTransform_.translate.z - 0.7f });
	particle_->Update();

	// カメラ
	followCamera_->Update();
	camera_.matView = followCamera_->GetCamera().matView;
	camera_.matProjection = followCamera_->GetCamera().matProjection;
	camera_.TransferMatrix();

	// 天球
	skydome_->Update();

	if (worldTransform_.translate.z >= 250.0f) {
		isDissolve_ = true;
	}

	if (isDissolve_) {
		postProcess_->SetDissolveParam(param_);
		param_.threshold += 0.02f;
	}
	
	if (param_.threshold >= 1.1f) {
		isDissolve_ = false;
		isDissolve2_ = true;
		worldTransform_.translate.z = 0.0f;
	}

	if (isDissolve2_) {
		postProcess_->SetDissolveParam(param_);
		param_.threshold -= 0.02f;
	}

	if (param_.threshold <= 0.0f && !isDissolve_) {
		param_.threshold = 0.0f;
		isDissolve2_ = false;
	}
}

void TitleScene::Draw()
{
	//spriteTitle_->Draw();

	//spriteWhite_->Draw();

	postProcess_->Draw();

	if (startATimer_ % 40 >= 20) {
		spritePushA_->Draw();
	}
}

void TitleScene::PostProcessDraw()
{
	postProcess_->PreDraw();

	skydome_->Draw(camera_);
	
	particle_->Draw(camera_);
	objectPlayer_->Draw(camera_);
	
	
	postProcess_->PostDraw();
}
