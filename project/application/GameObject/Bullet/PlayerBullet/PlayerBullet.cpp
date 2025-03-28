/**
* @file PlayerBullet.cpp
* @brief プレイヤー（自機）の弾
* @author 仁平 琉乃
*/

#include "PlayerBullet.h"

void PlayerBullet::Initialize()
{
	// object共通の初期化
	BaseObject::Initialize("Player/cube.obj", "TempTexture/white.png", ColliderType::Sphere);
	// collider設定
	collider_->SetCollosionAttribute(kCollisionAttributePlayerBullet);
	collider_->SetCollisionMask(kCollisionAttributeEnemy); // 当たる対象
	type_ = BulletType::Player;
	isDead_ = false;
	isActive_ = false;
}

void PlayerBullet::Update()
{
	Move(); // 移動
	BaseObject::Update(); // object共通の更新
	collider_->SetWorldPosition(GetWorldPosition()); // colliderにワールド座標を送る
	OnCollision(); // 当たったら

	// 時間で消滅
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void PlayerBullet::Draw(const Camera& camera)
{
	BaseObject::Draw(camera);
}

void PlayerBullet::ResetDeathTimer()
{
	deathTimer_ = kLifeTime_;
}

void PlayerBullet::Move()
{
	//移動
	Vector3 move{};
	move = object_->GetWorldTransform().translate + velocity_;
	object_->SetPosition(move);
}

void PlayerBullet::OnCollision()
{
	if (collider_->OnCollision()) {
		isDead_ = true;
	}	
}

Vector3 PlayerBullet::GetWorldPosition() const
{
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = object_->GetWorldTransform().matWorld.m[3][0];
	worldPos.y = object_->GetWorldTransform().matWorld.m[3][1];
	worldPos.z = object_->GetWorldTransform().matWorld.m[3][2];

	return worldPos;
}