#include "EnemyBullet.h"

void EnemyBullet::Initialize(uint32_t texHandle)
{
	object_ = std::make_unique<Object3DPlacer>();
	object_->Initialize();
	object_->SetModel("cube.obj");
	worldTransform_.Initialize();
	worldTransform_.scale = { 0.5f,0.5f,0.5f };	
	object_->SetWorldTransform(worldTransform_);
	object_->SetTexHandle(texHandle);

	SetCollosionAttribute(kCollisionAttributeEnemy);
	SetCollisionMask(kCollisionAttributePlayer); // 当たる対象
}

void EnemyBullet::Update()
{
	Move();
	worldTransform_.UpdateMatrix();
}

void EnemyBullet::Draw(Camera& camera)
{
	object_->Draw(camera);
}

void EnemyBullet::Move()
{
	//移動
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

Matrix4x4 EnemyBullet::Rotation(float angle, const Vector3& axis)
{
	Matrix4x4 result;
	float rad = angle * (std::numbers::pi_v<float> / 180.0f); // 角度をラジアンに変換
	float cosA = cosf(rad);
	float sinA = sinf(rad);

	// 回転軸の単位ベクトル
	Vector3 normalizedAxis = Normalize(axis);
	float x = normalizedAxis.x;
	float y = normalizedAxis.y;
	float z = normalizedAxis.z;

	result.m[0][0] = cosA + (1 - cosA) * x * x;
	result.m[0][1] = (1 - cosA) * x * y - sinA * z;
	result.m[0][2] = (1 - cosA) * x * z + sinA * y;
	result.m[0][3] = 0;

	result.m[1][0] = (1 - cosA) * y * x + sinA * z;
	result.m[1][1] = cosA + (1 - cosA) * y * y;
	result.m[1][2] = (1 - cosA) * y * z - sinA * x;
	result.m[1][3] = 0;

	result.m[2][0] = (1 - cosA) * z * x - sinA * y;
	result.m[2][1] = (1 - cosA) * z * y + sinA * x;
	result.m[2][2] = cosA + (1 - cosA) * z * z;
	result.m[2][3] = 0;

	result.m[3][0] = 0;
	result.m[3][1] = 0;
	result.m[3][2] = 0;
	result.m[3][3] = 1;

	return result;
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
