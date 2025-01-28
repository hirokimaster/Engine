#include "IGimmick.h"

IGimmick::~IGimmick()
{
}

void IGimmick::Initialize()
{
	object_ = std::make_unique<Object3DPlacer>();
	object_->Initialize();
	SetCollosionAttribute(kCollisionAttributeEnemyBullet);
	SetCollisionMask(kCollisionAttributePlayer); // 当たる対象
	SetType(ColliderType::Sphere); // 形状
}

void IGimmick::Update()
{
	object_->Update();
}

void IGimmick::Draw(Camera& camera)
{
	object_->Draw(camera);
}
