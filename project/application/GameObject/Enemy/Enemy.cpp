#include "Enemy.h"

void Enemy::Initialize(Object3DPlacer* object, uint32_t texHandle, const std::string& model)
{
	BaseObject::Initialize(object, texHandle, model); // 共通処理初期化
	worldTransform_.translate = { 0,0,80.0f };
	texHandleBullet_ = TextureManager::Load("resources/uvChecker.png"); // bulletの画像
	object->SetColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));

	SetCollosionAttribute(kCollisionAttributeEnemy);
	SetCollisionMask(kCollisionAttributePlayer); // 当たる対象
}

void Enemy::Update()
{
	BulletUpdate(); // 弾の更新処理
	BaseObject::Update(); // 共通の更新処理

#ifdef _DEBUG

	ImGui::Begin("Enemy");
	ImGui::DragFloat3("translate", &worldTransform_.translate.x, 0.1f, -100.0f, 100.0f);
	ImGui::End();

#endif
}

void Enemy::Draw(Camera& camera)
{
	BaseObject::Draw(camera); // 共通の描画処理

	// 弾の描画
	for (bulletsItr_ = bullets_.begin();
		bulletsItr_ != bullets_.end(); ++bulletsItr_) {
		(*bulletsItr_)->Draw(camera);
	}
}

void Enemy::OnCollision()
{
	isDead_ = true;
}

void Enemy::Fire()
{
	// 弾の速度
	const float kBulletSpeed = -2.0f;
	Vector3 velocity = { 0,0,kBulletSpeed };

	// 弾を生成し、初期化
	std::unique_ptr<EnemyBullet> bullet = std::make_unique<EnemyBullet>();
	std::unique_ptr<Object3DPlacer> objectBullet = std::make_unique<Object3DPlacer>();
	objectBullets_.push_back(std::move(objectBullet));
	bullet->Initialize(objectBullets_.back().get(), texHandleBullet_, "cube.obj");
	bullet->SetVelocity(velocity);
	bullet->SetPosition(GetWorldPosition());
	bullets_.push_back(std::move(bullet));

}

void Enemy::BulletUpdate()
{
	//発射タイマーをデクリメント
	--fireTimer_;

	if (fireTimer_ <= 0) {
		// 弾を発射
		Fire();
		// 発射タイマーの初期化
		fireTimer_ = kFireInterval_;
	}

	// 弾更新
	for (bulletsItr_ = bullets_.begin();
		bulletsItr_ != bullets_.end(); ++bulletsItr_) {
		(*bulletsItr_)->Update();
	}

	std::list<std::unique_ptr<Object3DPlacer>>::iterator objectBulletsItr = objectBullets_.begin();	// objectのイテレータ

	// デスフラグが立ったら要素を削除
	bullets_.remove_if([&objectBulletsItr, this](std::unique_ptr<EnemyBullet>& bullet) {
		if (bullet->GetIsDead()) {
			// 対応するbulletObjectを削除
			objectBulletsItr = objectBullets_.erase(objectBulletsItr);
			return true;
		}
		else {
			++objectBulletsItr;
			return false;
		}
		});

}

Vector3 Enemy::GetWorldPosition()
{
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.matWorld.m[3][0];
	worldPos.y = worldTransform_.matWorld.m[3][1];
	worldPos.z = worldTransform_.matWorld.m[3][2];

	return worldPos;
}
