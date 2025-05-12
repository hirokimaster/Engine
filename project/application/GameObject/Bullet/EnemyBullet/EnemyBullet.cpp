/**
* @file EnemyBullet.h
* @brief 敵の弾
* @author 仁平 琉乃
*/

#include "EnemyBullet.h"
#include "application/GameObject/Player/Player.h"

void EnemyBullet::Initialize()
{
	// object共通の初期化
	BaseInstancingObject::Initialize("Player/cube.obj", "TempTexture/white.png", ColliderType::Sphere);
	// collider設定
	collider_->SetCollosionAttribute(kCollisionAttributeEnemyBullet);
	collider_->SetCollisionMask(kCollisionAttributePlayer); // 当たる対象
	collider_->SetRadious(2.0f);
	type_ = BulletType::Enemy;
	isDead_ = false;
	isActive_ = true;
}

void EnemyBullet::Update()
{
	Move(); // 移動
	// object共通の更新処理
	BaseInstancingObject::Update();
	collider_->SetWorldPosition(GetWorldPosition()); // colliderにワールド座標を送る
	OnCollision(); // 当たったら

	// 時間で消滅
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void EnemyBullet::ResetDeathTimer()
{
	isMove_ = false;
	deathTimer_ = kLifeTime_;
}

void EnemyBullet::Move()
{
	Vector3 move = object_.lock()->worldTransform.translate + velocity_;
	SetPosition(move);
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
	worldPos.x = object_.lock()->worldTransform.matWorld.m[3][0];
	worldPos.y = object_.lock()->worldTransform.matWorld.m[3][1];
	worldPos.z = object_.lock()->worldTransform.matWorld.m[3][2];

	return worldPos;
}
