#include "BossEnemy.h"

void BossEnemy::Initialize(uint32_t texHandle)
{
	object_ = std::make_unique<Object3DPlacer>();
	object_->Initialize();
	object_->SetModel("cube.obj");
	worldTransform_.Initialize();
	object_->SetWorldTransform(worldTransform_);
	object_->SetTexHandle(texHandle);

	worldTransform_.translate = { 0,0,80.0f };

	SetCollosionAttribute(kCollisionAttributeEnemy);
	SetCollisionMask(kCollisionAttributePlayer); // 当たる対象
}

void BossEnemy::Update()
{
	worldTransform_.UpdateMatrix();
}

void BossEnemy::Draw(Camera& camera)
{
	object_->Draw(camera);
}
