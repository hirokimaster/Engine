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
	// particle
	particleManager_ = ParticleManager::GetInstance();
	particleManager_->Initialize();

	// objectManager
	objectManager_ = ObjectManager::GetInstance();
	objectManager_->Initialize();

	// loader
	loader_ = std::make_unique<Loader>();
	loader_->Record();
	loader_->ObjectRegister();

	// postEffect
	postEffect_ = std::make_unique<PostEffect>();
	postEffect_->Initialize();

	// シーン遷移
	transition_ = std::make_unique<FadeOut>();
	transition_->Initialize();
	GameManager::GetInstance()->SetSceneTransition(transition_.get());

	// テクスチャ読み込み
	LoadTextureFile();

	// lockOn
	lockOn_ = std::make_unique<LockOn>();

	// player
	player_ = std::make_unique<Player>();
	player_->Initialize();
	lockOn_->Initialize();
	player_->SetLockOn(lockOn_.get());

	// 障害物
	obstaclesManager_ = std::make_unique<ObstaclesManager>();
	obstaclesManager_->SetLoader(loader_.get());
	obstaclesManager_->Initialize();

	// 弾
	bulletObjectPool_ = std::make_unique<BulletObjectPool>();
	bulletObjectPool_->Initialize();
	player_->SetBulletObjectPool(bulletObjectPool_.get());

	// 敵
	enemyManager_ = std::make_unique<EnemyManager>();
	enemyManager_->SetPlayer(player_.get());
	enemyManager_->SetLoader(loader_.get());
	enemyManager_->SetBulletObjectPool(bulletObjectPool_.get());
	enemyManager_->Initialize();

	// ゲームの状態
	gameState_ = std::make_unique<GameStartState>();
	gameState_->Initialize();

	// カメラ
	cameraManager_ = CameraManager::GetInstance();
	cameraManager_->Initialize();
	cameraManager_->GetFollowCamera()->SetTarget(&player_->GetWorldTransform());
	cameraManager_->GetFollowCamera()->SetLockOn(lockOn_.get());
	isGameStart_ = true;

	// 天球
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize();

	// ゲームスプライト
	gameSprite_ = std::make_unique<GameSprite>();
	gameSprite_->Initialize();
	gameSprite_->SetGameScene(this);
	gameSprite_->SetPlayer(player_.get());

	timer_ = 60.0f;

	// collision
	collisionManager_ = std::make_unique<CollisionManager>();
}

void GameScene::Update()
{

	objectManager_->Update();

	// player
	player_->Update();
	lockOn_->UpdateReticle(cameraManager_->GetCamera(), player_->GetWorldPosition(), isGameStart_);

	// 弾
	bulletObjectPool_->Update();

	// enemy
	enemyManager_->Update();

	// 障害物
	obstaclesManager_->Update();

	// camera
	cameraManager_->Update();

	// lockOn
	lockOn_->Update(enemyManager_->GetEnemys(), cameraManager_->GetCamera());

	// collision
	Collision();

	// 天球
	skydome_->Update();

	// 敵の攻撃が当たったら
	if (player_->GetIsHitEnemyFire()) {
		postEffect_->ChangeState(std::make_unique<PlayerDamegeState>());
	}

	if (player_->GetIsDead()) {
		postEffect_->ChangeState(std::make_unique<GaussianState>());
		cameraManager_->StartShake(0.1f, 0.4f); // シェイクさせる
	}

	if (player_->GetWorldPosition().z >= 85000.0f && !isTransitionClear_) {
		isTransitionClear_ = true;
		transition_ = std::make_unique<FadeIn>();
		transition_->Initialize();
		GameManager::GetInstance()->SetSceneTransition(transition_.get());
		GameManager::GetInstance()->ChangeScene("CLEAR");
	}


	gameSprite_->Update();

	// postEffect更新
	postEffect_->Update();

	--timer_;
	if (timer_ <= 0.0f) {
		gameState_->Update();
	}

	// particle
	particleManager_->Update();
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

	skydome_->Draw(cameraManager_->GetCamera());


	// objectManager
	objectManager_->Draw(cameraManager_->GetCamera());

	// particle
	particleManager_->Draw(cameraManager_->GetCamera());

	// lockOn_(レティクル)
	lockOn_->Draw();

	postEffect_->GetPostProcess()->PostDraw();
}

void GameScene::Collision()
{
	collisionManager_->ColliderClear(); // colliderのリストをクリア

	collisionManager_->ColliderPush(player_->GetCollider()); // playerのcolliderをリストに追加

	for (const auto& bullet : bulletObjectPool_->GetBullets()) {
		if (bullet->GetIsActive()) {
			collisionManager_->ColliderPush(bullet->GetCollider()); // bulletのcolliderをリストに追加
		}
	}

	// enemy
	for (const auto& enemy : enemyManager_->GetEnemys()) {
		collisionManager_->ColliderPush(enemy->GetCollider()); // enemycolliderをリストに追加
	}

	// 障害物
	for (const auto& obstacles : obstaclesManager_->GetObstacles()) {
		collisionManager_->ColliderPush(obstacles->GetCollider()); // 障害物のcolliderをリストに追加
	}

	for (const auto& lasers : obstaclesManager_->GetLasers()) {
		collisionManager_->ColliderPush(lasers->GetCollider()); // 障害物のcolliderをリストに追加
	}

	collisionManager_->CheckAllCollision(); // 判定
}

void GameScene::LoadTextureFile()
{
	TextureManager::Load("resources/TempTexture/white2.png");
	TextureManager::Load("resources/TempTexture/white.png");
	TextureManager::Load("resources/Stage/laser.png");
	TextureManager::Load("resources/UI/RB.png");
	TextureManager::Load("resources/UI/RB2.png");
	TextureManager::Load("resources/UI/L.png");
	TextureManager::Load("resources/Player/smoke.png");
	TextureManager::Load("resources/UI/yes.png");
	TextureManager::Load("resources/UI/no.png");
	TextureManager::Load("resources/UI/continue.png");
	TextureManager::Load("resources/UI/select.png");
}

