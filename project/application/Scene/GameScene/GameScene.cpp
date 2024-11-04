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
	// postEffect
	isTransition_ = true;
	postProcess_ = std::make_unique<PostProcess>();
	postProcess_->Initialize();
	postProcess_->SetEffect(PostEffectType::Dissolve);
	param_.threshold = 1.0f;
	postProcess_->SetDissolveParam(param_);

	// テクスチャ読み込み
	LoadTextureFile();
	postProcess_->SetMaskTexture(texHandleMask_);
	spriteWhite_.reset(Sprite::Create(texHandleWhite_));

	camera_.Initialize();
	// lockOn
	lockOn_ = std::make_unique<LockOn>();
	// player
	player_ = std::make_unique<Player>();
	player_->Initialize(texHandlePlayer_);
	player_->SetPosition({ 0,0,50.0f });
	lockOn_->Initialize(player_->GetWorldTransform().translate);
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
	t = 0.0f;
	spriteEngage_.reset(Sprite::Create(texHandleEngage_, { 650.0f,250.0f }));
	spriteEngage_->SetAnchorPoint({ 0.5f,0.5f });

	// skyBox
	/*skyBox_ = std::make_unique<SkyBox>();
	skyBox_->Initialize();
	texHandleSkyBox_ = TextureManager::Load("resources/rostock_laage_airport_4k.dds");
	skyBox_->SetTexHandle(texHandleSkyBox_);
	worldTransformSkyBox_.Initialize();
	worldTransformSkyBox_.scale = { 500.0f,500.0f,500.0f };*/

	// loader
	uint32_t texhandle = TextureManager::Load("resources/TempTexture/white.png");
	loader_ = std::make_unique<Loader>();
	levelData_ = loader_->Load("level");
	loader_->SetPlayer(player_.get());
	loader_->SetTexHandle(texhandle);
	loader_->Arrangement(levelData_);


	// 天球
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize();

	// bossEnemy
	/*bossEnemy_ = std::make_unique<BossEnemy>();
	bossEnemy_->Initialize(texHandlePlayer_);
	bossEnemy_->SetPlayer(player_.get());*/

	// 仮のUI
	spriteLockOn_.reset(Sprite::Create(texHandleLockOn_, { 20.0f,100.0f }));
	spriteUnLock_.reset(Sprite::Create(texHandleUnLock_, { 20.0f,100.0f }));
	spriteAttack_.reset(Sprite::Create(texHandleAttack_, { 1000.0f , 100.0f }));

	// ゲームオーバー用
	texColor_ = { 1.0f,1.0f,1.0f,0.0f };
	spriteYes_.reset(Sprite::Create(texHandleYes_, { 400.0f,400.0f }, texColor_));
	spriteYes2_.reset(Sprite::Create(texHandleYes2_, { 400.0f,400.0f }, texColor_));
	spriteNo_.reset(Sprite::Create(texHandleNo_, { 750.0f,400.0f }, texColor_));
	spriteNo2_.reset(Sprite::Create(texHandleNo2_, { 750.0f,400.0f }, texColor_));
	spriteContinue_.reset(Sprite::Create(texHandleContinue_, { 450.0f,250.0f }, texColor_));
}

void GameScene::Update()
{
	// シーン遷移
	SceneTransition();

	// ゲームスタート演出
	StartGame();
	
	// player
	player_->Update();
	lockOn_->UpdateReticle(camera_, player_->GetWorldPosition());

	// loader
	loader_->Update();

	// bossEnemy
	//bossEnemy_->Update();

	// followCamera
	followCamera_->Update();
	camera_.matView = followCamera_->GetCamera().matView;
	camera_.matProjection = followCamera_->GetCamera().matProjection;
	camera_.TransferMatrix();

	// skyBox
	//worldTransformSkyBox_.UpdateMatrix();
	// lockOn
	lockOn_->Update(loader_->GetEnemys(), camera_);

	Collision();

	// 仮のクリア判定
	if (player_->GetWorldPosition().z >= 300.0f) {
		//isTransitionClear_ = true;
	}

	// 天球
	skydome_->Update();

	// ダメージエフェクト
	DamegeEffect();

	// ゲームオーバー
	GameOver();


#ifdef _DEBUG
	// カメラの座標
	ImGui::Begin("Camera");
	ImGui::SliderFloat3("CmeraTranslation ", &camera_.translate.x, -50.0f, 50.0f);
	ImGui::SliderFloat3("CmeraRotate ", &camera_.rotate.x, 0.0f, 10.0f);
	ImGui::End();

	ImGui::Begin("Player");
	ImGui::Text("position [x: %.3f ] [y: %.3f] [z: %.3f]", player_->GetWorldPosition().x,
		player_->GetWorldPosition().y, player_->GetWorldPosition().z);
	ImGui::End();

#endif

}

void GameScene::Draw()
{
	spriteWhite_->Draw();

	postProcess_->Draw();

	if (selectNo_ >= 1) {
		spriteNo_->Draw();
	}
	else {
		spriteNo2_->Draw();
	}

	if (selectNo_ <= 0) {
		spriteYes_->Draw();
	}
	else {
		spriteYes2_->Draw();
	}

	spriteContinue_->Draw();
	
}

void GameScene::PostProcessDraw()
{
	postProcess_->PreDraw();

	skydome_->Draw(camera_);

	// skyBox
	//skyBox_->Draw(worldTransformSkyBox_, camera_);
	loader_->Draw(camera_);
	// player
	player_->Draw(camera_);
	// bossEnemy
	//bossEnemy_->Draw(camera_);
	// lockOn_(レティクル)
	lockOn_->Draw();
	// 仮UI
	if (lockOn_->GetIsLockOnMode()) {
		spriteUnLock_->Draw();
	}
	else {
		spriteLockOn_->Draw();
	}

	spriteAttack_->Draw();

	if (engageColor_.w > 0.0f && t >= 0.7f) {
		spriteEngage_->Draw();

	}
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

	// bossEnemy
	//collisionManager_->ColliderPush(bossEnemy_.get()); // bossをリストに追加
	//// bossEnemyBullet
	//for (const auto& bossEnemyBullet : bossEnemy_->GetBullets()) {
	//	collisionManager_->ColliderPush(bossEnemyBullet.get()); // bossのbulletをリストに追加
	//}

	collisionManager_->CheckAllCollision(); // 判定
}

void GameScene::LoadTextureFile()
{
	texHandleMask_ = TextureManager::Load("resources/TempTexture/noise0.png");
	texHandleWhite_ = TextureManager::Load("resources/TempTexture/white2.png");
	texHandlePlayer_ = TextureManager::Load("resources/TempTexture/white.png");
	texHandleLockOn_ = TextureManager::Load("resources/UI/LockOn.png");
	texHandleUnLock_ = TextureManager::Load("resources/UI/unLock.png");
	texHandleAttack_ = TextureManager::Load("resources/UI/attack.png");
	texHandleEngage_ = TextureManager::Load("resources/UI/engage.png");

	// ゲームオーバー用
	texHandleYes_ = TextureManager::Load("resources/UI/yes.png");
	texHandleYes2_ = TextureManager::Load("resources/UI/yes2.png");
	texHandleNo_ = TextureManager::Load("resources/UI/no.png");
	texHandleNo2_ = TextureManager::Load("resources/UI/no2.png");
	texHandleContinue_ = TextureManager::Load("resources/UI/continue.png");
}

void GameScene::SceneTransition()
{
	// ゲームシーンに来た時
	if (isTransition_) {
		postProcess_->SetDissolveParam(param_);
		param_.threshold -= 0.01f;
		if (param_.threshold <= 0.0f) {
			isTransition_ = false;
			param_.threshold = 0.0f;
		}
	}

	// ゲームシーンからクリアへ
	if (isTransitionClear_) {
		postProcess_->SetDissolveParam(param_);
		param_.threshold += 0.01f;
		if (param_.threshold >= 1.2f) {
			isTransitionClear_ = false;
			title_ = true;
			GameManager::GetInstance()->ChangeScene("TITLE");
		}
	}
}

void GameScene::StartGame()
{
	if (!isTransition_ && t <= 1.0f) {
		Vector3 currentOffset = Lerp(offsetStart_, offsetEnd_, t);

		// 回転の補間
		Vector3 currentCameraRotate = Lerp(cameraRotateStart_, cameraRotateEnd_, t);
		currentCameraRotate.y = NormalizeRotation(currentCameraRotate.y);  // 必要なら正規化

		followCamera_->SetOffset(currentOffset);
		followCamera_->SetRotate(currentCameraRotate);

		t += 0.02f;
	}

	if (t >= 1.0f) {
		t = 1.0f;
		spriteEngage_->SetColor(engageColor_);
		engageColor_.w -= 0.03f;
	}

	if (engageColor_.w <= 0.0f) {
		engageColor_.w = 0.0f;
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

	if (!isPlayerIncurDamage_ && param_.threshold <= 0.0f) {
		postProcess_->SetEffect(PostEffectType::Dissolve);
		postProcess_->SetDissolveParam(param_);
		param_.threshold = 0.0f;
	}
}

void GameScene::GameOver()
{
	// 追従をやめる
	if (player_->GetDeadTimer() <= 0.0f) {
		followCamera_->SetTarget(nullptr);
		spriteContinue_->SetColor(texColor_);
		spriteYes_->SetColor(texColor_);
		spriteYes2_->SetColor(texColor_);
		spriteNo_->SetColor(texColor_);
		spriteNo2_->SetColor(texColor_);
		texColor_.w += 0.05f;
		postProcess_->SetEffect(PostEffectType::GaussianBlur);
	}

	if (texColor_.w >= 2.0f) {
		spriteYes_->SetColor(texColor_);
		spriteYes2_->SetColor(texColor_);
		spriteNo_->SetColor(texColor_);
		spriteNo2_->SetColor(texColor_);
		texColor_.w = 2.0f;
	}

	if (Input::GetInstance()->PressedButton(XINPUT_GAMEPAD_DPAD_LEFT) && selectNo_ == 1) {
		selectNo_ -= 1;
	}
	else if (Input::GetInstance()->PressedButton(XINPUT_GAMEPAD_DPAD_RIGHT) && selectNo_ == 0) {
		selectNo_ += 1;
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

