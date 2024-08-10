#include "EnemyBullet.h"
#include "application/GameObject/Player/Player.h"

void EnemyBullet::Initialize(uint32_t texHandle)
{
	object_ = std::make_unique<Object3DPlacer>();
	object_->Initialize();
	object_->SetModel("cube.obj");
	worldTransform_.Initialize();
	worldTransform_.scale = { 0.5f,0.5f,0.5f };
	object_->SetWorldTransform(worldTransform_);
	object_->SetTexHandle(texHandle);

	velocity_ = 1.2f * RandomDirection();

	SetCollosionAttribute(kCollisionAttributeEnemy);
	SetCollisionMask(kCollisionAttributePlayer); // 当たる対象
}

void EnemyBullet::Update()
{
	Move();
	BulletErase();
	worldTransform_.UpdateMatrix();
}

void EnemyBullet::Draw(Camera& camera)
{
	object_->Draw(camera);
}

void EnemyBullet::Move()
{

	//deltaTime_ += 1.0f / 60.0f;
	//// ターゲットへのベクトルを計算
	//Vector3 toTarget = player_->GetWorldPosition() - GetWorldPosition();
	//// 正規化
	//toTarget = Normalize(toTarget);

	//Vector3 curveVector = CalculateCurve(deltaTime_);

	//// 現在の速度方向と目標方向を補間して新しい方向を決定
	//Vector3 desiredDirection = Normalize(toTarget + curveVector);
	//velocity_ = (1.0f - 0.1f) * velocity_ + 0.1f * desiredDirection;
	//velocity_ = Normalize(velocity_);
	//velocity_ = 1.0f * velocity_;

	//移動
	//worldTransform_.translate = worldTransform_.translate + (deltaTime_ * velocity_);
	worldTransform_.translate = worldTransform_.translate + velocity_;

	//timeElapsed_ += 1.0f / 60.0f;

	worldTransform_.UpdateMatrix();
}

void EnemyBullet::BulletErase()
{
	// 時間で消滅
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	/*if (player_->GetWorldPosition().z - 5.0f  >= GetWorldPosition().z) {
		isDead_ = true;
	}*/
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

Vector3 EnemyBullet::CalculateCurve(float dt)
{
	// カーブの振幅と周波数を設定
	float curveAmplitude = 0.8f;
	float curveFrequency = 0.8f;

	// カーブベクトルを計算
	float curveOffset = std::sin(timeElapsed_ * curveFrequency) * curveAmplitude;
	return Vector3(0.0f, curveOffset, 0.0f); // Y軸方向のカーブ
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
