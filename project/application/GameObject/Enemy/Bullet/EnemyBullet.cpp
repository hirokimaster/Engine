#include "EnemyBullet.h"

void EnemyBullet::Initialize(Object3DPlacer* object, uint32_t texHandle, const std::string& model)
{
	BaseObject::Initialize(object, texHandle, model); // 共通処理初期化
	SetCollosionAttribute(kCollisionAttributeEnemy);
	SetCollisionMask(kCollisionAttributePlayer); // 当たる対象
}

void EnemyBullet::Update()
{
	Move();
	BaseObject::Update();  // 共通の更新処理
}

void EnemyBullet::Draw(Camera& camera)
{	
	BaseObject::Draw(camera); // 共通の描画処理
}

void EnemyBullet::Move()
{
	//移動
	//worldTransform_.translate = worldTransform_.translate + velocity_;



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

Vector3 EnemyBullet::GetWorldPosition()
{
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.matWorld.m[3][0];
	worldPos.y = worldTransform_.matWorld.m[3][1];
	worldPos.z = worldTransform_.matWorld.m[3][2];

	return worldPos;
}
