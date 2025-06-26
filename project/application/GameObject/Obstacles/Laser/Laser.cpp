/**
* @file Laser.cpp
* @brief レーザー(障害物)
* @author 仁平 琉乃
*/

#include "Laser.h"

void Laser::Initialize()
{
	TextureManager::Load("resources/Stage/laser.png");
	
	collider_ = std::make_unique<Collider>();
	collider_->SetType(ColliderType::AABB);
	// colliderの属性
	collider_->SetCollosionAttribute(kCollisionAttributeEnemyBullet);
	collider_->SetCollisionMask(kCollisionAttributePlayer); // 当たる対象

	// particleManagerから取ってくる
	particleManager_ = ParticleManager::GetInstance();
	if (type_ == LaserType::Side) {
		particle_ = particleManager_->GetParticle("laser_side", "Stage/laser.png");
	}
	else if (type_ == LaserType::Ver) {
		particle_ = particleManager_->GetParticle("laser_ver", "Stage/laser.png");
	}
	
	lifeTime_ = 60000.0f; // 60秒
	particle_->SetLifeTime(lifeTime_);

	worldTransform_.Initialize();
	worldTransform_.scale.z = 5.0f;
}

void Laser::Update()
{
	collider_->SetWorldPosition(GetWorldPosition());
	collider_->SetScale(worldTransform_.scale);
	OnCollision(); // 当たったら

	// パーティクルの更新
	if (type_ == LaserType::Side) {
		particle_->SetParticleParam(particleManager_->GetParam("laser_side"));
	}
	else if (type_ == LaserType::Ver) {
		particle_->SetParticleParam(particleManager_->GetParam("laser_ver"));
	}
	
	// アクティブにする
	particle_->SetIsActive(true);
	particle_->SetPosition(GetWorldPosition());

	worldTransform_.UpdateMatrix();
}

void Laser::OnCollision()
{
	if (collider_->OnCollision()) {
		// 当たった
		isHit_ = true;
	}
	else {
		isHit_ = false;
	}
	
}

Vector3 Laser::GetWorldPosition() const
{
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.matWorld.m[3][0];
	worldPos.y = worldTransform_.matWorld.m[3][1];
	worldPos.z = worldTransform_.matWorld.m[3][2];

	return worldPos;
}

