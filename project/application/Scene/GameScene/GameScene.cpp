/**
* @file GameScene.cpp
* @brief ゲームシーン
* @author 仁平 琉乃
*/

#include "GameScene.h"
#include "engine/ModelManager/ModelManager.h"

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
	player_->Initialize(texHandlePlayer_);
	lockOn_->Initialize();
	player_->SetLockOn(lockOn_.get());
	// rail
	rail_ = std::make_unique<Rail>();
	rail_->Initialize();
	rail_->SetPlayer(player_.get());

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
	t = 0.0f;

	// loader
	uint32_t texhandle = TextureManager::Load("resources/TempTexture/white.png");
	loader_ = std::make_unique<Loader>();
	loader_->SetPlayer(player_.get());
	loader_->SetTexHandle(texhandle);
	loader_->Arrangement();

	// 仮のUI
	spriteAttack_.reset(Sprite::Create(texHandleAttack_, { 1000.0f , 100.0f }));

	// ゲームオーバー用
	texColor_ = { 1.0f,1.0f,1.0f,0.0f };
	spriteYes_.reset(Sprite::Create(texHandleYes_, { 400.0f,400.0f }, texColor_));
	spriteNo_.reset(Sprite::Create(texHandleNo_, { 750.0f,400.0f }, texColor_));
	spriteContinue_.reset(Sprite::Create(texHandleContinue_, { 450.0f,250.0f }, texColor_));

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
		title_ = true;
		sceneTransition_->FadeIn("CLEAR");
	}

	// ゲームスタート演出
	StartGame();

	// player
	player_->Update();
	lockOn_->UpdateReticle(camera_, player_->GetWorldPosition(), isGameStart_);
	rail_->Update();

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

	collisionManager_->ColliderPush(player_.get()); // playerをリストに追加

	// playerBullet
	for (const auto& playerBullet : player_->GetBullets()) {
		collisionManager_->ColliderPush(playerBullet.get()); // playerBulletをリストに登録
	}

	// enemy
	for (const auto& enemy : loader_->GetEnemys()) {
		collisionManager_->ColliderPush(enemy.get()); // enemyをリストに登録

		for (const auto& enemyBullet : enemy->GetBullets()) {
			collisionManager_->ColliderPush(enemyBullet.get()); // enemybulletをリストに追加
		}
	}

	collisionManager_->CheckAllCollision(); // 判定
}

void GameScene::LoadTextureFile()
{
	texHandleWhite_ = TextureManager::Load("resources/TempTexture/white2.png");
	texHandlePlayer_ = TextureManager::Load("resources/TempTexture/white.png");
	texHandleAttack_ = TextureManager::Load("resources/UI/attack.png");

	// ゲームオーバー用
	texHandleYes_ = TextureManager::Load("resources/UI/yes.png");
	texHandleNo_ = TextureManager::Load("resources/UI/no.png");
	texHandleContinue_ = TextureManager::Load("resources/UI/continue.png");
}

void GameScene::StartGame()
{
	if (!isTransition_ && t <= 1.0f) {
		Vector3 currentOffset = Lerp(offsetStart_, offsetEnd_, t);

		// 回転の補間
		Vector3 currentCameraRotate = Lerp(cameraRotateStart_, cameraRotateEnd_, t);
		currentCameraRotate.y = NormalizeRotation(currentCameraRotate.y);

		followCamera_->SetOffset(currentOffset);
		followCamera_->SetRotate(currentCameraRotate);

		t += 0.02f;
	}

	if (t >= 1.0f) {
		isGameStart_ = true;
		RadialParam param = {
			.center = Vector2(0.5f,0.5f),
			.blurWidth = 0.001f,
		};
		postProcess_->SetEffect(PostEffectType::RadialBlur);
		postProcess_->SetRadialParam(param);
		t = 1.0f;
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

	if (effectTime_ > 0.0f && isPlayerIncurDamage_) {

		VignetteParam param = {
			.scale = 60.0f,
			.exponent = 0.3f
		};

		postProcess_->SetEffect(PostEffectType::Vignette);
		postProcess_->SetVignetteParam(param);
		followCamera_->StartShake(0.1f, 0.4f); // シェイクさせる
	}

	if (effectTime_ < 0.0f) {
		isPlayerIncurDamage_ = false;
	}

	if (!isPlayerIncurDamage_) {
		effectTime_ = 10.0f;
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

	if (Input::GetInstance()->PressedButton(XINPUT_GAMEPAD_DPAD_LEFT) && selectNo_ == 1) {
		selectNo_ -= 1;
	}
	else if (Input::GetInstance()->PressedButton(XINPUT_GAMEPAD_DPAD_RIGHT) && selectNo_ == 0) {
		selectNo_ += 1;
	}

	static float scaleTimer = 0.0f;
	const float scaleSpeed = 2.0f;
	const float scaleRange = 0.2f;

	// spriteNo,Yesのアニメーション
	scaleTimer += scaleSpeed * 1.0f / 60.0f;
	float scaleValue = 1.0f + scaleRange * sin(scaleTimer);

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

	if (texColor_.w >= 2.0f) {
		if (Input::GetInstance()->PressedButton(XINPUT_GAMEPAD_A) && selectNo_ >= 1) {
			GameManager::GetInstance()->ChangeScene("TITLE");
		}

		if (Input::GetInstance()->PressedButton(XINPUT_GAMEPAD_A) && selectNo_ <= 0) {
			Initialize();
		}
	}

}

