
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
	/*postProcess_ = std::make_unique<PostProcess>();
	postProcess_->Initialize();
	GameManager::GetInstance()->SetPostProcess(postProcess_.get());
	postProcess_->SetEffect(DepthOutline);*/

	camera_.Initialize();
	// lockOn
	lockOn_ = std::make_unique<LockOn>();
	lockOn_->Initialize();
	// player
	player_ = std::make_unique<Player>();
	texHandlePlayer_ = TextureManager::Load("resources/white.png");
	player_->Initialize(texHandlePlayer_);
	player_->SetPosition({ 0,0,50.0f });
	player_->SetLockOn(lockOn_.get());

	// enemy
	std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>();
	texHandleEnemy_ = TextureManager::Load("resources/white.png");
	enemy->Initialize(texHandleEnemy_);
	enemys_.push_back(std::move(enemy));

	// collision
	collisionManager_ = std::make_unique<CollisionManager>();
	// railCamera
	railCamera_ = std::make_unique<RailCamera>();
	railCamera_->Initialize(player_->GetWorldPosition(), { 0,0,0 });
	player_->SetParent(&railCamera_->GetWorldTransform());
	player_->SetRailCamera(railCamera_.get());
	// skyBox
	skyBox_ = std::make_unique<SkyBox>();
	skyBox_->Initialize();
	texHandleSkyBox_ = TextureManager::Load("resources/rostock_laage_airport_4k.dds");
	skyBox_->SetTexHandle(texHandleSkyBox_);
	worldTransformSkyBox_.Initialize();
	worldTransformSkyBox_.scale = { 500.0f,500.0f,500.0f };

	// loader
	uint32_t texhandle = TextureManager::Load("resources/uvChecker.png");
	loader_ = std::make_unique<Loader>();
	levelData_ = loader_->Load("level");
	loader_->SetTexHandle(texhandle);
	loader_->Arrangement(levelData_);

	// 仮のUI
	texHandleLockOn_ = TextureManager::Load("resources/LockOn.png");
	texHandleUnLock_ = TextureManager::Load("resources/unLock.png");
	texHandleAttack_ = TextureManager::Load("resources/attack.png");

	spriteLockOn_.reset(Sprite::Create(texHandleLockOn_, { 20.0f,100.0f }));
	spriteUnLock_.reset(Sprite::Create(texHandleUnLock_, { 20.0f,100.0f }));
	spriteAttack_.reset(Sprite::Create(texHandleAttack_, { 1000.0f , 100.0f }));
}

void GameScene::Update()
{
	// player
	player_->Update();
	lockOn_->UpdateReticle(camera_, player_->GetWorldPosition());

	RandomRespawn();

	// enemy
	for (enemysItr_ = enemys_.begin();
		enemysItr_ != enemys_.end(); ++enemysItr_) {

		(*enemysItr_)->Update();

	}

	// デスフラグが立ったら要素を削除
	enemys_.remove_if([](std::unique_ptr<Enemy>& enemy) {
		if (enemy->GetIsDead()) {
			return true;
		}
			return false;
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

	// 仮のクリア判定
	if (player_->GetWorldPosition().z >= 250.0f) {
		GameManager::GetInstance()->ChangeScene("CLEAR");
	}

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
	//postProcess_->Draw();
	// enemy
	for (enemysItr_ = enemys_.begin();
		enemysItr_ != enemys_.end(); ++enemysItr_) {

		(*enemysItr_)->Draw(camera_);
	}
	// skyBox
	//skyBox_->Draw(worldTransformSkyBox_, camera_);
	loader_->Draw(camera_);
	// player
	player_->Draw(camera_);
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
}

void GameScene::PostProcessDraw()
{
	//postProcess_->PreDraw();

	//// enemy
	//for (enemysItr_ = enemys_.begin();
	//	enemysItr_ != enemys_.end(); ++enemysItr_) {

	//	(*enemysItr_)->Draw(camera_);
	//}
	//// skyBox
	//skyBox_->Draw(worldTransformSkyBox_, camera_);
	//// player
	//player_->Draw(camera_);
	//// lockOn_(レティクル)
	//lockOn_->Draw();

	//postProcess_->PostDraw();
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

	if (spawnTimer_ >= 60) {
		std::mt19937 randomEngine(seed());
		std::uniform_real_distribution<float>distributionX(-12.0f, 12.0f);
		std::uniform_real_distribution<float>distributionY(0.0f, 5.0f);
		std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>();
		texHandleEnemy_ = TextureManager::Load("resources/white.png");
		enemy->Initialize(texHandleEnemy_);
		enemy->SetPosition(Vector3(float(distributionX(randomEngine)), float(distributionY(randomEngine)), 80.0f + railCamera_->GetWorldTransform().translate.z));
		enemys_.push_back(std::move(enemy));
		spawnTimer_ = 0;
	}
}
