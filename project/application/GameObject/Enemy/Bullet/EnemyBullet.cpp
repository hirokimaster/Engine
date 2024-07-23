#include "EnemyBullet.h"

void EnemyBullet::Initialize(uint32_t texHandle)
{
	object_ = std::make_unique<Object3DPlacer>();
	object_->Initialize();
	object_->SetModel("cube.obj");
	worldTransform_.Initialize();
	object_->SetWorldTransform(worldTransform_);
	object_->SetTexHandle(texHandle);

	SetCollosionAttribute(kCollisionAttributeEnemy);
	SetCollisionMask(kCollisionAttributePlayer); // 当たる対象
}

void EnemyBullet::Update()
{
	Move();
	worldTransform_.UpdateMatrix();
}

void EnemyBullet::Draw(Camera& camera)
{
	object_->Draw(camera);
}

void EnemyBullet::Move()
{
	//移動
	worldTransform_.translate = worldTransform_.translate + velocity_;

}

void EnemyBullet::BulletErase()
{
	// 時間で消滅
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void EnemyBullet::OnCollision()
{
	isDead_ = true;
}

Vector3 EnemyBullet::GetWorldPosition() const
{
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.matWorld.m[3][0];
	worldPos.y = worldTransform_.matWorld.m[3][1];
	worldPos.z = worldTransform_.matWorld.m[3][2];

	return worldPos;
}
