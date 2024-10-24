/**
* @file EnemyBullet.h
* @brief 敵の弾
* @author 仁平 琉乃
*/

#include "EnemyBullet.h"
#include "application/GameObject/Player/Player.h"

void EnemyBullet::Initialize(uint32_t texHandle, BulletType type)
{
	object_ = std::make_unique<Object3DPlacer>();
	object_->Initialize();
	object_->SetModel("Enemy/cube.obj");
	worldTransform_.Initialize();
	worldTransform_.scale = { 0.5f,0.5f,0.5f };
	object_->SetWorldTransform(worldTransform_);
	object_->SetTexHandle(texHandle);

	if (type == BulletType::Missile) {
		velocity_ = 1.2f * RandomDirection();
	}
	else {
		type;
	}

	SetCollosionAttribute(kCollisionAttributeEnemy);
	SetCollisionMask(kCollisionAttributePlayer); // 当たる対象
}

void EnemyBullet::Update(BulletType type)
{
	if (type == BulletType::Missile) {
		CurveBullet();
	}
	else {
		Move();
	}
	
	BulletErase();
	worldTransform_.UpdateMatrix();
}

void EnemyBullet::Draw(Camera& camera)
{
	object_->Draw(camera);
}

void EnemyBullet::Move()
{

	worldTransform_.translate = worldTransform_.translate + velocity_;

	worldTransform_.UpdateMatrix();
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

Vector3 EnemyBullet::RandomDirection()
{
	Vector3 result = Vector3(
		((float)std::rand() / RAND_MAX) * 2.0f - 1.0f,
		((float)std::rand() / RAND_MAX) * 2.0f - 1.0f,
		((float)std::rand() / RAND_MAX) * 2.0f - 1.0f);

	return result;
}

Vector3 EnemyBullet::CalculateCurve()
{
	// カーブの振幅と周波数を設定
	float curveAmplitude = 0.8f;
	float curveFrequency = 0.8f;

	// カーブベクトルを計算
	float curveOffset = std::sin(timeElapsed_ * curveFrequency) * curveAmplitude;
	return Vector3(0.0f, curveOffset, 0.0f); // Y軸方向のカーブ
}

void EnemyBullet::CurveBullet()
{

	// ターゲットへのベクトルを計算
	Vector3 toTarget = player_->GetWorldPosition() - GetWorldPosition();

	// 追尾を止める距離
	float distanceToTarget = Length(toTarget);
	float stopDistance = 10.0f;


	// 一定の距離以上の場合追尾する
	if (distanceToTarget > stopDistance) {
		// 正規化
		toTarget = Normalize(toTarget);

		Vector3 curveVector = CalculateCurve();

		// 今の速度方向と目標方向を補間して新しい方向を決める
		Vector3 desiredDirection = Normalize(toTarget + curveVector);
		velocity_ = (1.0f - 0.1f) * velocity_ + 0.1f * desiredDirection;
		velocity_ = Normalize(velocity_);
	}
	else {
		isDead_ = true;
	}

	// 移動
	worldTransform_.translate = worldTransform_.translate + (timeElapsed_ * velocity_);
	timeElapsed_ += 1.0f / 60.0f;
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
