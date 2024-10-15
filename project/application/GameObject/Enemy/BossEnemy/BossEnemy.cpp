#include "BossEnemy.h"

void BossEnemy::Initialize(uint32_t texHandle)
{
	object_ = std::make_unique<Object3DPlacer>();
	object_->Initialize();
	object_->SetModel("Enemy/cube.obj");
	worldTransform_.Initialize();
	object_->SetWorldTransform(worldTransform_);
	object_->SetTexHandle(texHandle);

	worldTransform_.scale = { 3.0f,3.0f,3.0f };
	worldTransform_.translate = { 0,0,100.0f };
	behaviorRequest_ = Behavior::kRoot;

	texHandleBullet_ = TextureManager::Load("resources/TempTexture/uvChecker.png");

	SetCollosionAttribute(kCollisionAttributeEnemy);
	SetCollisionMask(kCollisionAttributePlayer); // 当たる対象
}

void BossEnemy::Update()
{
	if (behaviorRequest_) {
		behavior_ = behaviorRequest_.value();
		switch (behavior_)
		{
		case Behavior::kRoot:
		default:
			//初期化
			BehaviorRootInitialize();
			break;
		}

		behaviorRequest_ = std::nullopt;
	}

	switch (behavior_)
	{
	case Behavior::kRoot:
	default:
		BehaviorRootUpdate();
		break;
	}

	// 弾の更新処理
	UpdateBullet();

	// 移動
	Move();

	worldTransform_.UpdateMatrix();

	/*ImGui::Begin("bossPosition");
	ImGui::Text("position [x: %.3f ] [y: %.3f] [z: %.3f]", worldTransform_.translate.x,
		worldTransform_.translate.y, worldTransform_.translate.z);
	ImGui::End();*/
}

void BossEnemy::Draw(Camera& camera)
{
	object_->Draw(camera);

	// 弾の描画
	for (const auto& bullet : bullets_) {
		bullet->Draw(camera);
	}
}

void BossEnemy::Move()
{
	Vector3 playerWorldPosition = player_->GetWorldPosition(); // playerのワールド座標
	Vector3 offset = { 0.0f,5.0f, 40.0f}; // playerから一定距離保つためのoffset

	worldTransform_.translate.y = playerWorldPosition.y + offset.y;
	worldTransform_.translate.z = playerWorldPosition.z + offset.z; // playerが動くからその分移動する
}

void BossEnemy::UsuallyFire()
{
	const float kBulletSpeed = 0.2f; // 弾の速度
	Vector3 playerWorldPosition = player_->GetWorldPosition(); // playerのワールド座標
	Vector3 diff = playerWorldPosition - GetWorldPosition(); // 差分ベクトル
	diff = Normalize(diff); // 正規化
	Vector3 velocity = kBulletSpeed * diff; // 速度ベクトル
	// 弾の生成
	std::unique_ptr<EnemyBullet> bullet = std::make_unique<EnemyBullet>();
	bullet->Initialize(texHandleBullet_, BulletType::Normal);
	bullet->SetPosition(GetWorldPosition());
	bullet->SetVelocity(velocity);
	bullets_.push_back(std::move(bullet));
}

void BossEnemy::UpdateBullet()
{
	// 弾の更新
	for (const auto& bullet : bullets_) {
		bullet->Update(BulletType::Normal);
	}

	// デスフラグの立ったやつを消す
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		if (bullet->GetIsDead()) {
			bullet.reset();
			return true;
		}
		return false;
		});

}

void BossEnemy::BehaviorRootInitialize()
{
	fireTimer_ = 60;
	fireInterval_ = 60;
}

void BossEnemy::BehaviorRootUpdate()
{
	//発射タイマーをデクリメント
	--fireTimer_;

	if (fireTimer_ <= 0) {
		// 弾を発射 (通常攻撃)
		UsuallyFire();
		// 発射タイマーの初期化
		fireTimer_ = fireInterval_;
	}
}

Vector3 BossEnemy::GetWorldPosition() const
{
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.matWorld.m[3][0];
	worldPos.y = worldTransform_.matWorld.m[3][1];
	worldPos.z = worldTransform_.matWorld.m[3][2];

	return worldPos;
}

void BossEnemy::OnCollision()
{

}
