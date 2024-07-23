#include "PlayerBullet.h"

void PlayerBullet::Initialize(Object3DPlacer* object, uint32_t texHandle, const std::string& model)
{
	BaseObject::Initialize(object, texHandle, model); // 共通処理初期化
	SetCollosionAttribute(kCollisionAttributePlayer);
	SetCollisionMask(kCollisionAttributeEnemy); // 当たる対象

	if (lockOn_ && lockOn_->GetIsLockOnMode()) {
		// ホーミング弾
		worldTransform_.rotate.y = std::atan2(velocity_.x, velocity_.z);

		Vector3 velocityXZ = { velocity_.x, 0.0f,velocity_.z };

		float veloXZ = Length(velocityXZ);

		worldTransform_.rotate.z = std::atan2(-velocity_.y, veloXZ);
	}

	worldTransform_.scale = { 0.5f,0.5f,0.5f };
	
}

void PlayerBullet::Update()
{
	Move(); // 移動
	BulletErase(); // 弾を削除
	BaseObject::Update();  // 共通の更新処理

	if (lockOn_ && lockOn_->GetIsLockOnMode()) {
		// ホーミング弾
		std::list<const Enemy*> targets = lockOn_->GetTarget();
		if (!targets.empty()) {
			for (std::list<const Enemy*>::iterator itr = targets.begin();
				itr != targets.end(); ++itr) {
				Vector3 toEnemy = (*itr)->GetWorldPosition() - GetWorldPosition();
				Normalize(toEnemy);
				Normalize(velocity_);
				float t = 0.01f;
				velocity_ = SLerp(toEnemy, GetWorldPosition(), t);
			}
		}

		worldTransform_.rotate.y = std::atan2(velocity_.x, velocity_.z);

		Vector3 velocityXZ = { velocity_.x, 0.0f, velocity_.z };

		float veloXZ = Length(velocityXZ);

		worldTransform_.rotate.z = std::atan2(-velocity_.y, veloXZ);
	}
   
}

void PlayerBullet::Draw(Camera& camera)
{
	BaseObject::Draw(camera); // 共通の描画処理
}

void PlayerBullet::Move()
{
	//移動
	worldTransform_.translate = worldTransform_.translate + velocity_;
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
	isDead_ = true;
}

Vector3 PlayerBullet::GetWorldPosition() const
{
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.matWorld.m[3][0];
	worldPos.y = worldTransform_.matWorld.m[3][1];
	worldPos.z = worldTransform_.matWorld.m[3][2];

	return worldPos;
}
