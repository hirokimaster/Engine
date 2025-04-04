#include "Laser.h"

void Laser::Initialize()
{
	// object共通の初期化
	BaseObject::Initialize("Player/cube.obj", "TempTexture/white.png", ColliderType::AABB);
	object_->SetColor({ 10.0f,10.0f,10.0f,10.0f });
	
	// colliderの属性
	collider_->SetCollosionAttribute(kCollisionAttributeEnemyBullet);
	collider_->SetCollisionMask(kCollisionAttributePlayer); // 当たる対象
}

void Laser::Update()
{
	BaseObject::Update(); // object共通の更新処理
	collider_->SetWorldPosition(GetWorldPosition());
	collider_->SetScale(object_->GetWorldTransform().scale);
	OnCollision(); // 当たったら
}

void Laser::Draw(const Camera& camera)
{
	BaseObject::Draw(camera);
}

void Laser::OnCollision()
{
	if (collider_->OnCollision()) {
		// 当たった
		isHit_ = true;
	}
	
}

Vector3 Laser::GetWorldPosition() const
{
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = object_->GetWorldTransform().matWorld.m[3][0];
	worldPos.y = object_->GetWorldTransform().matWorld.m[3][1];
	worldPos.z = object_->GetWorldTransform().matWorld.m[3][2];

	return worldPos;
}

