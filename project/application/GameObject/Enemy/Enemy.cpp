#include "Enemy.h"

void Enemy::Initialize(Object3DPlacer* object, uint32_t texHandle, const std::string& model)
{
	BaseObject::Initialize(object, texHandle, model); // 共通処理初期化
	worldTransform_.translate = { 0,0,20.0f };

	SetCollosionAttribute(kCollisionAttributeEnemy);
	SetCollisionMask(kCollisionAttributePlayer); // 当たる対象
}

void Enemy::Update()
{
	BaseObject::Update(); // 共通の更新処理
}

void Enemy::Draw(Camera& camera)
{
	BaseObject::Draw(camera); // 共通の描画処理
}

void Enemy::OnCollision()
{
	isDead_ = true;	
}

void Enemy::Dead()
{
}

Vector3 Enemy::GetWorldPosition()
{
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.matWorld.m[3][0];
	worldPos.y = worldTransform_.matWorld.m[3][1];
	worldPos.z = worldTransform_.matWorld.m[3][2];

	return worldPos;
}
