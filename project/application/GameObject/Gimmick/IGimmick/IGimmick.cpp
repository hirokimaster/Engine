#include "IGimmick.h"

IGimmick::~IGimmick()
{
}

void IGimmick::Initialize()
{
	object_ = std::make_unique<Object3DPlacer>();
	object_->Initialize();
	worldTransform_.Initialize();
	SetCollosionAttribute(kCollisionAttributeEnemyBullet);
	SetCollisionMask(kCollisionAttributePlayer); // 当たる対象
	SetType(ColliderType::AABB); // 形状
}

void IGimmick::Update()
{
	worldTransform_.UpdateMatrix();
}

void IGimmick::Draw(Camera& camera)
{
	object_->Draw(camera);
}
