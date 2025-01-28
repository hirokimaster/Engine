#include "Laser.h"

void Laser::Initialize()
{
	// 共通部分の初期化
	BaseObject::Initialize();
	object_->SetModel("LevelEditorObj/laser.obj");
	object_->SetTexHandle(TextureManager::GetTexHandle("TempTexture/mount.jpg"));
	SetCollosionAttribute(kCollisionAttributeEnemyBullet);
	SetCollisionMask(kCollisionAttributePlayer); // 当たる対象
	SetType(ColliderType::Sphere); // 形状
}

void Laser::Update()
{
	// 共通部分の更新
	BaseObject::Update();
}

void Laser::Draw(Camera& camera)
{
	BaseObject::Draw(camera);
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

void Laser::OnCollision()
{
	// 当たった
	isHit_ = true;
}
