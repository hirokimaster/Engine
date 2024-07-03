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
	ModelResources::GetInstance()->LoadModel(); // 使うモデルをロードしておく
	camera_.Initialize();
	// player
	playerManager_ = std::make_unique<PlayerManager>();
	playerManager_->Initialize();
	// enemy
	enemyManager_ = std::make_unique<EnemyManager>();
	enemyManager_->Initialize();
	// collision
	collisionManager_ = std::make_unique<CollisionManager>();
	// railCamera
	railCamera_ = std::make_unique<RailCamera>();
	railCamera_->Initialize(playerManager_->GetPlayerPosition(), {0,0,0});
	playerManager_->SetParent(&railCamera_->GetWorldTransform());
	// skyBox
	skyBox_ = std::make_unique<SkyBox>();
	skyBox_->Initialize();
	texHandleSkyBox_ = TextureManager::Load("resources/rostock_laage_airport_4k.dds");
	skyBox_->SetTexHandle(texHandleSkyBox_);
	worldTransformSkyBox_.Initialize();
}

void GameScene::Update()
{
	// player
	playerManager_->Update();
	// enemy
	enemyManager_->Update();
	// railCamera
	railCamera_->Update();
	camera_.matView = railCamera_->GetCamera().matView;
	camera_.matProjection = railCamera_->GetCamera().matProjection;
	camera_.TransferMatrix();
	// skyBox
	worldTransformSkyBox_.UpdateMatrix();

	Collision();
}

void GameScene::Draw()
{
	// player
	playerManager_->Draw(camera_);
	// enemy
	enemyManager_->Draw(camera_);
	// skyBox
	//skyBox_->Draw(worldTransformSkyBox_, camera_);
}

void GameScene::PostProcessDraw()
{
}

void GameScene::Collision()
{
	collisionManager_->ColliderClear(); // colliderのリストをクリア

	playerManager_->ColliderPush(collisionManager_.get()); // player関係のcolliderをリストに登録
	enemyManager_->ColliderPush(collisionManager_.get()); // enemy関係のcolliderをリストに登録

	collisionManager_->CheckAllCollision(); // 判定
}
