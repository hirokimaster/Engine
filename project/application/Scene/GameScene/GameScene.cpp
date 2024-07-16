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

	postProcess_ = std::make_unique<PostProcess>();
	postProcess_->Initialize();
	GameManager::GetInstance()->SetPostProcess(postProcess_.get());
	postProcess_->SetEffect(DepthOutline);

	camera_.Initialize();
	// lockOn
	lockOn_ = std::make_unique<LockOn>();
	// player
	player_ = std::make_unique<Player>();
	objectPlayer_ = std::make_unique<Object3DPlacer>();
	texHandlePlayer_ = TextureManager::Load("resources/white.png");
	player_->Initialize(objectPlayer_.get(), texHandlePlayer_, "cube.obj");
	player_->SetPosition({ 0,0,50.0f });
	
	// enemy
	std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>();
	std::unique_ptr<Object3DPlacer> objectEnemy = std::make_unique<Object3DPlacer>();
	objectEnemys_.push_back(std::move(objectEnemy));
	texHandleEnemy_ = TextureManager::Load("resources/white.png");
	enemy->Initialize(objectEnemys_.back().get(), texHandleEnemy_, "cube.obj");
	enemys_.push_back(std::move(enemy));
	// collision
	collisionManager_ = std::make_unique<CollisionManager>();
	// railCamera
	railCamera_ = std::make_unique<RailCamera>();
	railCamera_->Initialize(player_->GetWorldPosition(), {0,0,0});
	player_->SetParent(&railCamera_->GetWorldTransform());
	// skyBox
	skyBox_ = std::make_unique<SkyBox>();
	skyBox_->Initialize();
	texHandleSkyBox_ = TextureManager::Load("resources/rostock_laage_airport_4k.dds");
	skyBox_->SetTexHandle(texHandleSkyBox_);
	worldTransformSkyBox_.Initialize();
	worldTransformSkyBox_.scale = { 500.0f,500.0f,500.0f };
}

void GameScene::Update()
{
	player_->SetLockOn(lockOn_.get());
	lockOn_->SetPlayer(player_.get());
	// player
	player_->Update();
	player_->UpdateReticle(camera_);

	RandomRespawn();

	// enemy
	for (enemysItr_ = enemys_.begin();
		enemysItr_ != enemys_.end(); ++enemysItr_) {

		(*enemysItr_)->Update();

	}

	std::list<std::unique_ptr<Object3DPlacer>>::iterator objectEnemysItr = objectEnemys_.begin();	// objectのイテレータ

	// デスフラグが立ったら要素を削除
	enemys_.remove_if([&objectEnemysItr, this](std::unique_ptr<Enemy>& enemy) {
		if (enemy->GetIsDead()) {
			// 対応するbulletObjectを削除
			objectEnemysItr = objectEnemys_.erase(objectEnemysItr);
			return true;
		}
		else {
			++objectEnemysItr;
			return false;
		}
		});

	// railCamera
	railCamera_->Update();
	camera_.matView = railCamera_->GetCamera().matView;
	camera_.matProjection = railCamera_->GetCamera().matProjection;
	camera_.TransferMatrix();
	// skyBox
	worldTransformSkyBox_.UpdateMatrix();
	// lockOn
	lockOn_->Update(enemys_, camera_);

	Collision();

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
	postProcess_->Draw();

}

void GameScene::PostProcessDraw()
{
	postProcess_->PreDraw();

	// enemy
	for (enemysItr_ = enemys_.begin();
		enemysItr_ != enemys_.end(); ++enemysItr_) {

		(*enemysItr_)->Draw(camera_);
	}
	// skyBox
	skyBox_->Draw(worldTransformSkyBox_, camera_);
	// player
	player_->Draw(camera_);
	
	postProcess_->PostDraw();
}

void GameScene::Collision()
{
	collisionManager_->ColliderClear(); // colliderのリストをクリア

	collisionManager_->ColliderPush(player_.get()); // playerをリストに追加

	for (auto playerBulletsItr = player_->GetBullets().begin();
		playerBulletsItr != player_->GetBullets().end(); ++playerBulletsItr) {
		collisionManager_->ColliderPush((*playerBulletsItr).get()); // playerbulletをリストに追加
	}

	for (enemysItr_ = enemys_.begin();
		enemysItr_ != enemys_.end(); ++enemysItr_) {

		collisionManager_->ColliderPush((*enemysItr_).get()); // enemyをリストに登録

		for (auto enemyBulletsItr = (*enemysItr_)->GetBullets().begin();
			enemyBulletsItr != (*enemysItr_)->GetBullets().end(); ++enemyBulletsItr) {
			collisionManager_->ColliderPush((*enemyBulletsItr).get()); // enemybulletをリストに追加
		}
	}

	collisionManager_->CheckAllCollision(); // 判定
}

void GameScene::RandomRespawn()
{
	spawnTimer_++;
	std::random_device seed;

	if (spawnTimer_ >= 90) {
		std::mt19937 randomEngine(seed());
		std::uniform_real_distribution<float>distribution(-20.0f, 20.0f);
		std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>();
		std::unique_ptr<Object3DPlacer> objectEnemy = std::make_unique<Object3DPlacer>();
		objectEnemys_.push_back(std::move(objectEnemy));
		texHandleEnemy_ = TextureManager::Load("resources/white.png");
		enemy->Initialize(objectEnemys_.back().get(), texHandleEnemy_, "cube.obj");
		enemy->SetPosition(Vector3(float(distribution(randomEngine)), float(distribution(randomEngine)), 80.0f + railCamera_->GetWorldTransform().translate.z));
		enemys_.push_back(std::move(enemy));
		spawnTimer_ = 0;
	}
}
