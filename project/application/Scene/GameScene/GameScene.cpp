
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
	// テクスチャ読み込み
	LoadTextureFile();

	// postEffect
	isTransition_ = true;
	postProcess_ = std::make_unique<PostProcess>();
	postProcess_->Initialize();
	postProcess_->SetEffect(PostEffectType::Dissolve);
	postProcess_->SetMaskTexture(texHandleMask_);
	param_.threshold = 1.0f;
	postProcess_->SetDissolveParam(param_);
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
	Vector3 offset = { 0,2.0f,-40.0f };
	followCamera_->SetOffset(offset);
	followCamera_->SetTarget(&player_->GetWorldTransform());
	followCamera_->SetLockOn(lockOn_.get());

	// skyBox
	/*skyBox_ = std::make_unique<SkyBox>();
	skyBox_->Initialize();
	texHandleSkyBox_ = TextureManager::Load("resources/rostock_laage_airport_4k.dds");
	skyBox_->SetTexHandle(texHandleSkyBox_);
	worldTransformSkyBox_.Initialize();
	worldTransformSkyBox_.scale = { 500.0f,500.0f,500.0f };*/

	// loader
	uint32_t texhandle = TextureManager::Load("resources/TempTexture/uvChecker.png");
	loader_ = std::make_unique<Loader>();
	levelData_ = loader_->Load("level");
	loader_->SetPlayer(player_.get());
	loader_->SetTexHandle(texhandle);
	loader_->Arrangement(levelData_);

	// bossEnemy
	/*bossEnemy_ = std::make_unique<BossEnemy>();
	bossEnemy_->Initialize(texHandlePlayer_);
	bossEnemy_->SetPlayer(player_.get());*/

	// 仮のUI
	spriteLockOn_.reset(Sprite::Create(texHandleLockOn_, { 20.0f,100.0f }));
	spriteUnLock_.reset(Sprite::Create(texHandleUnLock_, { 20.0f,100.0f }));
	spriteAttack_.reset(Sprite::Create(texHandleAttack_, { 1000.0f , 100.0f }));
}

void GameScene::Update()
{
	// シーン遷移
	SceneTransition();

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
	if (player_->GetWorldPosition().z >= 1050.0f) {
		isTransitionClear_ = true;
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
	spriteWhite_->Draw();

	postProcess_->Draw();
}

void GameScene::PostProcessDraw()
{
	postProcess_->PreDraw();

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
			GameManager::GetInstance()->ChangeScene("CLEAR");
		}
	}
}
