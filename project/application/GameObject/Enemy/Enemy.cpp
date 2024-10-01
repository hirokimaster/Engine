#include "Enemy.h"
#include "application/GameObject/Player/Player.h"

void Enemy::Initialize(uint32_t texHandle)
{
	object_ = std::make_unique<Object3DPlacer>();
	object_->Initialize();
	object_->SetModel("cube.obj");
	worldTransform_.Initialize();
	object_->SetWorldTransform(worldTransform_);
	object_->SetTexHandle(texHandle);

	worldTransform_.translate = { 0,0,80.0f };
	texHandleBullet_ = TextureManager::Load("resources/uvChecker.png"); // bulletの画像
	object_->SetColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	bulletType_ = Missile;

	SetCollosionAttribute(kCollisionAttributeEnemy);
	SetCollisionMask(kCollisionAttributePlayer); // 当たる対象
}

void Enemy::Update()
{
	//BulletUpdate(); // 弾の更新処理
	worldTransform_.UpdateMatrix();

	// 時間で消滅
	/*if (--deathTimer_ <= 0) {
		isDead_ = true;
	}*/

#ifdef _DEBUG

	ImGui::Begin("Enemy");
	ImGui::DragFloat3("translate", &worldTransform_.translate.x, 0.1f, -100.0f, 100.0f);
	ImGui::End();

#endif
}

void Enemy::Draw(Camera& camera)
{
	object_->Draw(camera);

	// 弾の描画
	for (const auto& bullet : bullets_) {
		bullet->Draw(camera);
	}
}

void Enemy::OnCollision()
{
	isDead_ = true;
}

void Enemy::Fire()
{
}

void Enemy::BulletUpdate()
{
	//発射タイマーをデクリメント
	--fireTimer_;

	if (bulletType_ == Spiral) {
		isFire_ = true;
	}

	if (isFire_) {
		FireSpiral(0.5f, 60, 0.05f);
		--spiralTimer_;
	}

	if (spiralTimer_ <= 0.0f) {
		isFire_ = false;
	}

	if (fireTimer_ <= 0) {

		if (bulletType_ == Radial) {
			// 弾を発射
			FireRadial(20);
		}
		else if (bulletType_ == Normal) {
			Fire();
		}
		else if (bulletType_ == Missile) {
			FireMissile(3);
		}

		// 発射タイマーの初期化
		fireTimer_ = kFireInterval_;
	}

	// 弾更新
	for (const auto& bullet : bullets_) {
		bullet->Update(bulletType_);
	}

	// デスフラグが立ったら要素を削除
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		if (bullet->GetIsDead()) {

			return true;
		}
		return false;
		});

}

void Enemy::FireRadial(uint32_t bulletCount)
{
	// 弾の速度
	const float kBulletSpeed = -0.5f;

	// 弾幕の角度範囲
	float angleStep = 360.0f / bulletCount;

	const float kRadius = 0.1f; // 半径

	for (uint32_t i = 0; i < bulletCount; ++i) {
		float angle = i * angleStep * (std::numbers::pi_v<float> / 180.0f);
		Vector3 velocity = { cosf(angle) * kRadius, sinf(angle) * kRadius, kBulletSpeed };

		std::unique_ptr<EnemyBullet> bullet = std::make_unique<EnemyBullet>();
		bullet->Initialize(texHandleBullet_, Radial);
		bullet->SetVelocity(velocity);
		bullet->SetPosition(GetWorldPosition());
		bullets_.push_back(std::move(bullet));
	}
}

void Enemy::FireSpiral(float spiralSpeed, uint32_t bulletCount, float delayBetweenBullets)
{
	static uint32_t bulletIndex = 0; // 弾のインデックス
	static float timeLastShot = 0.0f; // 発射間隔に使う

	// 弾発射タイミングの制御
	timeLastShot += deltaTime_; // deltaTimeはフレームごとの時間差分

	if (timeLastShot >= delayBetweenBullets)
	{
		timeLastShot = 0.0f;

		// 弾の速度
		const float kBulletSpeed = -1.0f;

		// 螺旋の角度
		float angle = bulletIndex * spiralSpeed;

		// 速度ベクトルの計算
		float x = std::cos(angle);
		float y = std::sin(angle);
		Vector3 velocity = { x, y, 7.0f };
		velocity = Normalize(velocity);
		velocity.z = kBulletSpeed * velocity.z;

		// 弾を生成し、初期化
		std::unique_ptr<EnemyBullet> bullet = std::make_unique<EnemyBullet>();
		bullet->Initialize(texHandleBullet_,Spiral);
		bullet->SetVelocity(velocity);
		bullet->SetPosition(GetWorldPosition());

		// 弾をリストに追加
		bullets_.push_back(std::move(bullet));

		// 次の弾の角度とインデックスを更新
		bulletIndex = (bulletIndex + 1) % bulletCount;
	}

}

void Enemy::FireMissile(uint32_t bulletCount)
{
	for (uint32_t i = 0; i < bulletCount; ++i) {
		// 弾を生成し、初期化
		std::unique_ptr<EnemyBullet> bullet = std::make_unique<EnemyBullet>();
		bullet->Initialize(texHandleBullet_, Missile);
		bullet->SetPlayer(player_);
		bullet->SetPosition(GetWorldPosition());
		bullets_.push_back(std::move(bullet));
	}
}

Vector3 Enemy::GetWorldPosition() const
{
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.matWorld.m[3][0];
	worldPos.y = worldTransform_.matWorld.m[3][1];
	worldPos.z = worldTransform_.matWorld.m[3][2];

	return worldPos;
}
