/**
* @file PlayerBullet.cpp
* @brief プレイヤー（自機）の弾
* @author 仁平 琉乃
*/

#include "PlayerBullet.h"

void PlayerBullet::Initialize(uint32_t texHandle)
{
	object_ = std::make_unique<Object3DPlacer>();
	object_->Initialize();
	object_->SetModel("Player/cube.obj");
	worldTransform_.Initialize();
	object_->SetWorldTransform(worldTransform_);
	object_->SetTexHandle(texHandle);

	SetCollosionAttribute(kCollisionAttributePlayerBullet);
	SetCollisionMask(kCollisionAttributeEnemy); // 当たる対象

	worldTransform_.scale = { 0.5f,0.5f,0.5f };

	// particle
	texHandleSmoke_ = TextureManager::Load("resources/TempTexture/circle2.png");
	particleManager_ = ParticleManager::GetInstance();
	/*particleManager_->StartEditor("bulletTrajectory");
	particleManager_->CreateParticle("bulletTrajectory", "Player/plane.obj", texHandleSmoke_);
	particleManager_->ApplyParticleInfo("bulletTrajectory",id_);*/
}

void PlayerBullet::Update()
{
	Move(); // 移動
	BulletErase(); // 弾を削除
	--particleTimer_;
	if (particleTimer_ < 10) {
		//particleManager_->Clear("bulletTrajectory", id_);
	}
	else {
		//particleManager_->Update("bulletTrajectory", id_);
		//particleManager_->SetPosition("bulletTrajectory", GetWorldPosition(), id_);
	}
	
	worldTransform_.UpdateMatrix();
}

void PlayerBullet::Draw(Camera& camera)
{
	object_->Draw(camera);
	if (particleTimer_ > 10) {
		//particleManager_->Draw("bulletTrajectory", camera, id_);
	}	
}

void PlayerBullet::Move()
{
	//移動
	worldTransform_.translate = worldTransform_.translate + velocity_;
}

void PlayerBullet::BulletErase()
{
	// 時間で消滅
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

}

void PlayerBullet::OnCollision()
{
	isDead_ = true;
}

Vector3 PlayerBullet::GetWorldPosition() const
{
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.matWorld.m[3][0];
	worldPos.y = worldTransform_.matWorld.m[3][1];
	worldPos.z = worldTransform_.matWorld.m[3][2];

	return worldPos;
}