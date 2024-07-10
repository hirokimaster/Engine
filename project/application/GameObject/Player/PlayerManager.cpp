#include "PlayerManager.h"

void PlayerManager::Initialize()
{
	player_ = std::make_unique<Player>();
	objectPlayer_ = std::make_unique<Object3DPlacer>();
	texHandlePlayer_ = TextureManager::Load("resources/white.png");
	player_->Initialize(objectPlayer_.get(), texHandlePlayer_, "cube.obj");
	player_->SetPosition({ 0,0,50.0f });
}

void PlayerManager::Update(const Camera& camera)
{
	player_->Update();
	player_->UpdateReticle(camera);
}

void PlayerManager::Draw(Camera& camera)
{
	player_->Draw(camera);
}

void PlayerManager::ColliderPush(CollisionManager* collision)
{
	collision->ColliderPush(player_.get()); // playerをリストに追加

	for (auto playerBulletsItr = player_->GetBullets().begin();
		playerBulletsItr != player_->GetBullets().end(); ++playerBulletsItr) {
		collision->ColliderPush((*playerBulletsItr).get()); // playerbulletをリストに追加
	}
}
