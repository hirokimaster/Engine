#include "Wall.h"

void Wall::Initialize()
{
	// object共通の初期化
	BaseObject::Initialize("Player/cube.obj", "TempTexture/noise0.png", ColliderType::AABB);

	// colliderの属性
	collider_->SetCollosionAttribute(kCollisionAttributeEnemyBullet);
	collider_->SetCollisionMask(kCollisionAttributePlayer); // 当たる対象
}

void Wall::Update()
{
	BaseObject::Update(); // object共通の更新処理
	collider_->SetWorldPosition(GetWorldPosition());
	collider_->SetScale(object_->GetWorldTransform().scale);
	OnCollision(); // 当たったら
}

void Wall::Draw(const Camera& camera)
{
	BaseObject::Draw(camera);
}

Vector3 Wall::GetWorldPosition() const
{
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = object_->GetWorldTransform().matWorld.m[3][0];
	worldPos.y = object_->GetWorldTransform().matWorld.m[3][1];
	worldPos.z = object_->GetWorldTransform().matWorld.m[3][2];

	return worldPos;
}

void Wall::OnCollision()
{
	if(collider_->OnCollision()) {
		isHit_ = true;
	}
}
