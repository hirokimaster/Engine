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
	// postEffect
	postEffect_ = std::make_unique<PostEffect>();
	postEffect_->Initialize();

	isTransition_ = true;
	sceneTransition_ = SceneTransition::GetInstance();

	// テクスチャ読み込み
	LoadTextureFile();

	// lockOn
	lockOn_ = std::make_unique<LockOn>();

	// player
	player_ = std::make_unique<Player>();
	player_->Initialize();
	lockOn_->Initialize();
	player_->SetLockOn(lockOn_.get());

	// collision
	collisionManager_ = std::make_unique<CollisionManager>();

	// 追従カメラ
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

	// ゲームスタート
	isGameStart_ = false;
	sceneTimer_ = 100;

	// 天球
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize();

	// ゲームスプライト
	gameSprite_ = std::make_unique<GameSprite>();
	gameSprite_->Initialize();
	gameSprite_->SetGameScene(this);
	gameSprite_->SetPlayer(player_.get());
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
	lockOn_->UpdateReticle(followCamera_->GetCamera(), player_->GetWorldPosition(), isGameStart_);
	
	// loader
	loader_->Update();

	// followCamera
	followCamera_->Update();

	// lockOn
	lockOn_->Update(loader_->GetEnemys(), followCamera_->GetCamera());

	Collision();

	// 天球
	skydome_->Update();

	// 敵の攻撃が当たったら
	if (player_->GetIsHitEnemyFire()) {
		postEffect_->ChangeState(std::make_unique<PlayerDamegeState>());
	}
	// postEffect更新
	postEffect_->Update();

	// ゲームオーバー
	GameOver();
}

void GameScene::Draw()
{
	postEffect_->GetPostProcess()->Draw();

	gameSprite_->Draw();

	player_->DrawUI();
}

void GameScene::PostProcessDraw()
{
	postEffect_->GetPostProcess()->PreDraw();

	skydome_->Draw(followCamera_->GetCamera());

	loader_->Draw(followCamera_->GetCamera());
	// player
	player_->Draw(followCamera_->GetCamera());

	// lockOn_(レティクル)
	lockOn_->Draw();

	sceneTransition_->Draw();

	postEffect_->GetPostProcess()->PostDraw();
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
	TextureManager::Load("resources/UI/RB2.png");
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
		rotateParam_ = 1.0f;
	}

	isGameStart_ = true;
}

void GameScene::GameOver()
{
	// 追従をやめる
	if (player_->GetDeadTimer() <= 0.0f) {
		followCamera_->SetTarget(nullptr);
		
		for (auto& enemys : loader_->GetEnemys()) {
			enemys->SetIsDead(true);
		}
	}

}

