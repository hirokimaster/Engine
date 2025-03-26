/**
* @file PlayerBullet.cpp
* @brief プレイヤー（自機）の弾
* @author 仁平 琉乃
*/

#include "PlayerBullet.h"

void PlayerBullet::Initialize(uint32_t texHandle)
{
	object_ = std::make_unique<Object3DPlacer>();
	object_->Initialize();
	object_->SetModel("Player/cube.obj");
	object_->SetScale({ 0.3f,0.3f });
	object_->SetTexHandle(texHandle);

	// collider設定
	collider_ = std::make_unique<Collider>();
	collider_->SetCollosionAttribute(kCollisionAttributePlayerBullet);
	collider_->SetCollisionMask(kCollisionAttributeEnemy); // 当たる対象
	collider_->SetType(ColliderType::Sphere); // どの形状でとるか
}

void PlayerBullet::Update()
{
	Move(); // 移動
	BulletErase(); // 弾を削除
	object_->Update();
	collider_->SetWorldPosition(GetWorldPosition()); // colliderにワールド座標を送る
	OnCollision(); // 当たったら
}

void PlayerBullet::Draw(const Camera& camera)
{
	object_->Draw(camera);
}

void PlayerBullet::Move()
{
	//移動
	Vector3 move{};
	move = object_->GetWorldTransform().translate + velocity_;
	object_->SetPosition(move);
}

void PlayerBullet::BulletErase()
{
	// 時間で消滅
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

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