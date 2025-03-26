#include "FixedEnemy.h"

void FixedEnemy::Initialize()
{
	// object共通の初期化
	BaseObject::Initialize("LevelEditorObj/enemy.obj", "TempTexture/noise0.png", ColliderType::Sphere);
	// コライダーの属性設定
	collider_->SetCollosionAttribute(kCollisionAttributeEnemy);	  // 自分の属性
	collider_->SetCollisionMask(kCollisionAttributePlayerBullet); // 当たる対象
}

void FixedEnemy::Update()
{
    // object共通の更新処理
	BaseObject::Update();
}

void FixedEnemy::Draw(const Camera& camera)
{
	// object共通の描画処理
	BaseObject::Draw(camera);
}

void FixedEnemy::Fire()
{
	// ここに攻撃書く
}

void FixedEnemy::OnCollision()
{
	if (collider_->OnCollision()) {
		// ここに衝突した時の処理書く

	}
}

Vector3 FixedEnemy::GetWorldPosition() const
{
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = object_->GetWorldTransform().matWorld.m[3][0];
	worldPos.y = object_->GetWorldTransform().matWorld.m[3][1];
	worldPos.z = object_->GetWorldTransform().matWorld.m[3][2];

	return worldPos;
}

