/**
* @file GameScene.cpp
* @brief ゲームシーン
* @author 仁平 琉乃
*/

#include "GameScene.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

void GameScene::Initialize()
{
	// ポストエフェクトの初期化
	isTransition_ = true;
	postProcess_ = std::make_unique<PostProcess>();
	postProcess_->Initialize();
	postProcess_->SetEffect(PostEffectType::Vignette);

	sceneTransition_ = SceneTransition::GetInstance();

	// テクスチャ読み込み
	LoadTextureFile();

	camera_.Initialize();
	// lockOn
	lockOn_ = std::make_unique<LockOn>();
	// player
	player_ = std::make_unique<Player>();
	player_->Initialize(TextureManager::GetTexHandle("TempTexture/white.png"));
	lockOn_->Initialize();
	player_->SetLockOn(lockOn_.get());
	// collision
	collisionManager_ = std::make_unique<CollisionManager>();

	/*------------------------
			追従カメラ
	--------------------------*/
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();
	followCamera_->SetOffset(offsetStart_);
	followCamera_->SetRotate(cameraRotateStart_);
	followCamera_->SetTarget(&player_->GetWorldTransform());
	followCamera_->SetLockOn(lockOn_.get());
	rotateParam_ = 0.0f;

	// loader
	TextureManager::Load("resources/TempTexture/white.png");
	loader_ = std::make_unique<Loader>();
	loader_->SetPlayer(player_.get());
	loader_->SetTexHandle(TextureManager::GetTexHandle("TempTexture/white.png"));
	loader_->Arrangement();

	// 仮のUI
	spriteAttack_.reset(Sprite::Create(TextureManager::GetTexHandle("UI/RB.png"),{ 1000.0f , 500.0f }));
	spriteAttack_->SetScale({ 2.0f,2.0f,2.0f });
	spriteMove_.reset(Sprite::Create(TextureManager::GetTexHandle("UI/L.png"),{ 240.0f,500.0f }));
	spriteMove_->SetScale({ 2.0f,2.0f,2.0f });

	// ゲームオーバー用
	texColor_ = { 1.0f,1.0f,1.0f,0.0f };
	spriteYes_.reset(Sprite::Create(TextureManager::GetTexHandle("UI/yes.png"), { 400.0f,400.0f }, texColor_));
	spriteNo_.reset(Sprite::Create(TextureManager::GetTexHandle("UI/no.png"), { 750.0f,400.0f }, texColor_));
	spriteContinue_.reset(Sprite::Create(TextureManager::GetTexHandle("UI/continue.png"), { 450.0f,250.0f }, texColor_));

	// ゲームスタート
	isGameStart_ = false;
	sceneTimer_ = 100;

	// 天球
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize();
}

void GameScene::Update()
{
	// シーン遷移
	// ゲームシーンに来た時
	if (isTransition_) {
		sceneTransition_->FadeOut();
	}

	--sceneTimer_;

	if (sceneTimer_ <= 0) {
		isTransition_ = false;
		sceneTimer_ = 0;
	}

	// ゲームシーンからクリアへ
	if (isTransitionClear_) {
		sceneTransition_->SetPreScene(PreScene::Game);
		sceneTransition_->FadeIn("CLEAR");
	}

	// ゲームスタート演出
	StartGame();

	// player
	player_->Update();
	lockOn_->UpdateReticle(camera_, player_->GetWorldPosition(), isGameStart_);
	
	// loader
	loader_->Update();

	// followCamera
	float cameraRotateZ = player_->GetRotate().z;
	cameraRotateZ = std::clamp(cameraRotateZ, -0.1f, 0.1f);
	followCamera_->SetRotateZ(-cameraRotateZ);

	followCamera_->Update();
	camera_.matView = followCamera_->GetCamera().matView;
	camera_.matProjection = followCamera_->GetCamera().matProjection;
	camera_.TransferMatrix();

	// lockOn
	lockOn_->Update(loader_->GetEnemys(), camera_);

	Collision();

	// 天球
	skydome_->Update();

	// ダメージエフェクト
	DamegeEffect();

	// ゲームオーバー
	GameOver();

	// クリア条件
	if (player_->GetWorldPosition().z >= 8500.0f) {
		isTransitionClear_ = true;
	}

#ifdef _DEBUG
	// カメラの座標
	ImGui::Begin("Camera");
	ImGui::SliderFloat3("CmeraTranslation ", &camera_.translate.x, -50.0f, 50.0f);
	ImGui::SliderFloat3("CmeraRotate ", &camera_.rotate.x, 0.0f, 10.0f);
	ImGui::End();
#endif

}

void GameScene::Draw()
{
	postProcess_->Draw();

	spriteNo_->Draw();

	spriteYes_->Draw();

	spriteContinue_->Draw();

	spriteAttack_->Draw();

	spriteMove_->Draw();

}

void GameScene::PostProcessDraw()
{
	postProcess_->PreDraw();

	skydome_->Draw(camera_);

	loader_->Draw(camera_);
	// player
	player_->Draw(camera_);

	// lockOn_(レティクル)
	lockOn_->Draw();

	sceneTransition_->Draw();

	postProcess_->PostDraw();

}

void GameScene::Collision()
{
	collisionManager_->ColliderClear(); // colliderのリストをクリア

	collisionManager_->ColliderPush(player_->GetCollider()); // playerのcolliderをリストに追加

	// playerBullet
	for (const auto& playerBullet : player_->GetBullets()) {
		collisionManager_->ColliderPush(playerBullet->GetCollider()); // playerBulletをリストに登録
	}

	// enemy
	for (const auto& enemy : loader_->GetEnemys()) {
		collisionManager_->ColliderPush(enemy->GetCollider()); // enemyをリストに登録

		for (const auto& enemyBullet : enemy->GetBullets()) {
			collisionManager_->ColliderPush(enemyBullet->GetCollider()); // enemybulletをリストに追加
		}
	}

	// laser
	for (const auto& laser : loader_->GetLasers()) {
		collisionManager_->ColliderPush(laser->GetCollider()); // laserをリストに登録
	}

	collisionManager_->CheckAllCollision(); // 判定
}

void GameScene::LoadTextureFile()
{
	TextureManager::Load("resources/TempTexture/white2.png");
	TextureManager::Load("resources/TempTexture/white.png");
	TextureManager::Load("resources/UI/RB.png");
	TextureManager::Load("resources/UI/L.png");

	// ゲームオーバー用
	TextureManager::Load("resources/UI/yes.png");
	TextureManager::Load("resources/UI/no.png");
	TextureManager::Load("resources/UI/continue.png");
}

void GameScene::StartGame()
{
	// シーン遷移が終わったかつ補間が終わってない時
	if (!isTransition_ && rotateParam_ <= 1.0f) {
		// カメラの位置を補間する
		Vector3 currentOffset = Lerp(offsetStart_, offsetEnd_, rotateParam_);
		// 回転の補間をする
		Vector3 currentCameraRotate = Lerp(cameraRotateStart_, cameraRotateEnd_, rotateParam_);
		// 正規化
		currentCameraRotate.y = NormalizeRotation(currentCameraRotate.y);
		// カメラに適用
		followCamera_->SetOffset(currentOffset);
		followCamera_->SetRotate(currentCameraRotate);
		rotateParam_ += 0.02f;
	}

	// 補間が終わったら
	if (rotateParam_ >= 1.0f) {
		// ゲーム開始
		isGameStart_ = true;
		// ブラーをかける
		RadialParam param = {
			.center = Vector2(0.5f,0.5f),
			.blurWidth = 0.005f,
		};
		postProcess_->SetEffect(PostEffectType::RadialBlur);
		postProcess_->SetRadialParam(param);
		rotateParam_ = 1.0f;
	}
}

void GameScene::DamegeEffect()
{
	// プレイヤーがダメージを受けたら
	if (player_->GetIsHitEnemyFire()) {
		isPlayerIncurDamage_ = true;
	}

	if (isPlayerIncurDamage_) {
		--effectTime_;
	}
	else {
		effectTime_ = 10.0f;
	}

	// effectTimerが0になるまでにビネットをかける
	if (effectTime_ > 0.0f && isPlayerIncurDamage_) {

		VignetteParam param = {
			.scale = 60.0f,
			.exponent = 0.3f
		};

		postProcess_->SetEffect(PostEffectType::Vignette);
		postProcess_->SetVignetteParam(param);
		followCamera_->StartShake(0.1f, 0.4f); // シェイクさせる
	}

	// ダメージエフェクト終わり
	if (effectTime_ < 0.0f) {
		isPlayerIncurDamage_ = false;
	}

}

void GameScene::GameOver()
{
	// 追従をやめる
	if (player_->GetDeadTimer() <= 0.0f) {
		followCamera_->SetTarget(nullptr);
		spriteContinue_->SetColor(texColor_);
		spriteYes_->SetColor(texColor_);
		spriteNo_->SetColor(texColor_);
		texColor_.w += 0.05f;
		postProcess_->SetEffect(PostEffectType::GaussianBlur);
		for (auto& enemys : loader_->GetEnemys()) {
			enemys->SetIsDead(true);
		}
	}

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
		spriteYes_->SetScale({ scaleValue, scaleValue, scaleValue });
		spriteNo_->SetScale({ 1.0f, 1.0f, 1.0f });
	}
	else if (selectNo_ == 1) {
		spriteNo_->SetScale({ scaleValue, scaleValue, scaleValue });
		spriteYes_->SetScale({ 1.0f, 1.0f, 1.0f });
	}
	else {
		spriteYes_->SetScale({ 1.0f, 1.0f, 1.0f });
		spriteNo_->SetScale({ 1.0f, 1.0f, 1.0f });
	}

	// コンティニューするならリスタートやめるならタイトル
	if (texColor_.w >= 2.0f) {
		if (Input::GetInstance()->PressedButton(XINPUT_GAMEPAD_A) && selectNo_ >= 1) {
			GameManager::GetInstance()->ChangeScene("TITLE");
		}

		if (Input::GetInstance()->PressedButton(XINPUT_GAMEPAD_A) && selectNo_ <= 0) {
			Initialize();
		}
	}

}

