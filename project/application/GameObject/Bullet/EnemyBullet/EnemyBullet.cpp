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
	BaseObject::Initialize("Player/cube.obj", "TempTexture/white.png", ColliderType::Sphere);
	object_->SetScale({ 100.0f,100.0f,100.0f });
	object_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	// collider設定
	collider_->SetCollosionAttribute(kCollisionAttributeEnemyBullet);
	collider_->SetCollisionMask(kCollisionAttributePlayer); // 当たる対象
	collider_->SetRadious(2.0f);
	type_ = BulletType::Enemy;
	isDead_ = false;
	isActive_ = true;
	// particle
	particleManager_ = ParticleManager::GetInstance();
}

void EnemyBullet::Update()
{
	Move(); // 移動
	// object共通の更新処理
	BaseObject::Update();
	collider_->SetWorldPosition(GetWorldPosition()); // colliderにワールド座標を送る
	OnCollision(); // 当たったら

	// 時間で消滅
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	// particle
	if (isActive_ && !isMove_) {
		particle_ = particleManager_->GetParticle("bulletTrajectory", "Player/smoke.png");
		particle_->SetIsActive(true);
		isMove_ = true;
	}

	if (particle_) {
		particle_->SetPosition(GetWorldPosition());
	}
}

void EnemyBullet::Draw(const Camera& camera)
{
	BaseObject::Draw(camera);
}

void EnemyBullet::ResetDeathTimer()
{
	isMove_ = false;
	deathTimer_ = kLifeTime_;
}

void EnemyBullet::Move()
{
	Vector3 move = object_->GetWorldTransform().translate + velocity_;
	object_->SetPosition(move);
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
	worldPos.x = object_->GetWorldTransform().matWorld.m[3][0];
	worldPos.y = object_->GetWorldTransform().matWorld.m[3][1];
	worldPos.z = object_->GetWorldTransform().matWorld.m[3][2];

	return worldPos;
}
