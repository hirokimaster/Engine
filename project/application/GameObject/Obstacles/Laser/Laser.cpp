/**
* @file Laser.cpp
* @brief レーザー(障害物)
* @author 仁平 琉乃
*/

#include "Laser.h"

void Laser::Initialize()
{
	TextureManager::Load("resources/Stage/laser.png");
	// object共通の初期化
	BaseInstancingObject::Initialize("Stage/laser.obj", "Stage/laser.png", ColliderType::AABB);
	object_.lock()->color = { 10.0f,10.0f,10.0f,10.0f };
	
	// colliderの属性
	collider_->SetCollosionAttribute(kCollisionAttributeEnemyBullet);
	collider_->SetCollisionMask(kCollisionAttributePlayer); // 当たる対象
}

void Laser::Update()
{
	BaseInstancingObject::Update(); // object共通の更新処理
	collider_->SetWorldPosition(GetWorldPosition());
	collider_->SetScale(object_.lock()->worldTransform.scale);
	OnCollision(); // 当たったら
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
	worldPos.x = object_.lock()->worldTransform.matWorld.m[3][0];
	worldPos.y = object_.lock()->worldTransform.matWorld.m[3][1];
	worldPos.z = object_.lock()->worldTransform.matWorld.m[3][2];

	return worldPos;
}

