#include "FixedEnemy.h"
#include "application/GameObject/Player/Player.h"

void FixedEnemy::Initialize()
{
	// object共通の初期化
	BaseObject::Initialize("LevelEditorObj/fixedEnemy.obj", "TempTexture/noise0.png", ColliderType::Sphere);
	object_->SetColor({ 0.0f,0.0f,0.0f,1.0f });
	// コライダーの属性設定
	collider_->SetCollosionAttribute(kCollisionAttributeEnemy);	  // 自分の属性
	collider_->SetCollisionMask(kCollisionAttributePlayer); // 当たる対象
	collider_->SetRadious(8.0f); // コライダーのサイズ 
	// パーティクル
	particleManager_ = ParticleManager::GetInstance();
	// 影
	shadow_ = std::make_unique<PlaneProjectionShadow>();
	shadow_->Initialize("LevelEditorObj/fixedEnemy.obj", &object_->GetWorldTransform());
	bulletSpeed_ = 10.0f;
}

void FixedEnemy::Update()
{
	// object共通の更新処理
	BaseObject::Update();
	collider_->SetWorldPosition(GetWorldPosition());
	// 衝突
	OnCollision();
	// playerが越えて見えなくなったら消す
	if (GetWorldPosition().z <= player_->GetWorldPosition().z) {
		isDead_ = true;
	}

	if (isHit_ && !isExploded_) {
		particle_ = particleManager_->GetParticle("explosion", "Player/smoke.png");
		isExploded_ = true;
		shadow_.reset();
	}

	// particleの位置
	if (particle_) {
		particle_->SetIsActive(true);
		particle_->SetPosition(object_->GetWorldTransform().translate);
	}

	// 影
	if (shadow_) {
		shadow_->Update();
	}

	//// 発射間隔つける
	//if (GetWorldPosition().z - player_->GetWorldPosition().z <= 5000.0f) {
	//	// 発射タイマーをデクリメント
	//	--fireTimer_;
	//	if (fireTimer_ <= 0) {
	//		// 弾を発射
	//		Fire();
	//		// 発射タイマーの初期化
	//		fireTimer_ = kFireInterval_;
	//	}
	//}

	// 弾更新
	for (const auto& bullet : bullets_) {
		bullet->Update();
	}

	// デスフラグが立ったら要素を削除
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		if (bullet->GetIsDead()) {

			return true;
		}
		return false;
		});

#ifdef _DEBUG
	ImGui::Begin("enemy");
	ImGui::Text("position = %f : %f : %f", object_->GetWorldTransform().translate.x,
		object_->GetWorldTransform().translate.y, object_->GetWorldTransform().translate.z);
	ImGui::End();
#endif
}

void FixedEnemy::Draw(const Camera& camera)
{
	if (!isHit_) {
		// object共通の描画処理
		BaseObject::Draw(camera);
		// 影
		shadow_->Draw(camera);

		// 弾の描画
		for (const auto& bullet : bullets_) {
			bullet->Draw(camera);
		}
	}

}

void FixedEnemy::Fire()
{
	// playerがいなかったらそもそも撃つ対象がいない
	if (player_) {
		Vector3 playerWorldPos = player_->GetWorldPosition(); // 自キャラのワールド座標を取得
		Vector3 enemyWorldPos = GetWorldPosition(); // 敵キャラのワールド座標を取得
		Vector3 diff = Subtract(playerWorldPos, enemyWorldPos); // 差分ベクトルを求める
		diff = Normalize(diff); // 正規化
		Vector3 velocity = Multiply(bulletSpeed_, diff); // ベクトルの速度

		// 弾を生成して初期化
		// プールから取ってくる
		//IBullet* baseBullet = bulletObjectPool_->GetBullet("enemy");
		// 取ってこれたかチェックする

		std::unique_ptr<EnemyBullet> bullet = std::make_unique<EnemyBullet>();
		bullet->Initialize();
		bullet->SetPosition(GetWorldPosition());
		bullet->SetVelocity(velocity);
		bullet->SetIsActive(true);
		bullets_.push_back(std::move(bullet));

	}
}

void FixedEnemy::OnCollision()
{
	if (collider_->OnCollision()) {
		// ここに衝突した時の処理書く
		isHit_ = true;
	}
}

Vector3 FixedEnemy::GetWorldPosition() const
{
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = object_->GetWorldTransform().matWorld.m[3][0];
	worldPos.y = object_->GetWorldTransform().matWorld.m[3][1];
	worldPos.z = object_->GetWorldTransform().matWorld.m[3][2];

	return worldPos;
}

