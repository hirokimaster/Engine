/**
* @file Wall.cpp
* @brief 壁(障害物)
* @author 仁平 琉乃
*/

#include "Wall.h"

void Wall::Initialize()
{
	// object共通の初期化
	BaseInstancingObject::Initialize("Stage/wall.obj", "TempTexture/noise0.png", ColliderType::AABB);

	// colliderの属性
	collider_->SetCollosionAttribute(kCollisionAttributeEnemy);
	collider_->SetCollisionMask(kCollisionAttributePlayer); // 当たる対象
}

void Wall::Update()
{
	BaseInstancingObject::Update(); // object共通の更新処理
	collider_->SetWorldPosition(GetWorldPosition());
	collider_->SetScale(object_.lock()->worldTransform.scale);
	OnCollision(); // 当たったら
}

Vector3 Wall::GetWorldPosition() const
{
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = object_.lock()->worldTransform.matWorld.m[3][0];
	worldPos.y = object_.lock()->worldTransform.matWorld.m[3][1];
	worldPos.z = object_.lock()->worldTransform.matWorld.m[3][2];

	return worldPos;
}

void Wall::OnCollision()
{
	if(collider_->OnCollision()) {
		isHit_ = true;
	}
}
