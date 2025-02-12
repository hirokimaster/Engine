#include "Laser.h"

void Laser::Initialize()
{
	object_ = std::make_unique<Object3DPlacer>();
	object_->Initialize();
	object_->SetModel("LevelEditorObj/laser.obj");
	object_->SetTexHandle(TextureManager::GetTexHandle("TempTexture/mount.jpg"));
	object_->SetScale({ 0.1f,0.1f,0.1f });
	
	// collider設定
	collider_ = std::make_unique<Collider>();
	collider_->SetCollosionAttribute(kCollisionAttributeEnemyBullet);
	collider_->SetCollisionMask(kCollisionAttributePlayer); // 当たる対象
	collider_->SetType(ColliderType::Sphere); // 形状
}

void Laser::Update()
{
	object_->Update();
	collider_->SetWorldPosition(GetWorldPosition());
	collider_->SetScale(GetScale());
	OnCollision(); // 当たったら
}

void Laser::Draw(Camera& camera)
{
	object_->Draw(camera);
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
	if (collider_->OnCollision()) {
		// 当たった
		isHit_ = true;
	}
	
}
