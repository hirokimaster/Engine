/**
* @file FixedEnemy.cpp
* @brief 動かない敵
* @author 仁平 琉乃
*/

#include "FixedEnemy.h"
#include "application/GameObject/Player/Player.h"

void FixedEnemy::Initialize()
{
	// object共通の初期化
	BaseInstancingObject::Initialize("LevelEditorObj/fixedEnemy.obj", "TempTexture/noise0.png", ColliderType::Sphere);
	object_.lock()->color = {0.0f,0.0f,0.0f,1.0f};
	// コライダーの属性設定
	collider_->SetCollosionAttribute(kCollisionAttributeEnemy);	  // 自分の属性
	collider_->SetCollisionMask(kCollisionAttributePlayer); // 当たる対象
	collider_->SetRadious(8.0f); // コライダーのサイズ 
	// パーティクル
	particleManager_ = ParticleManager::GetInstance();
	bulletSpeed_ = 10.0f;
}

void FixedEnemy::Update()
{
	// object共通の更新処理
	BaseInstancingObject::Update();
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
	}

	// particleの位置
	if (particle_) {
		particle_->SetIsActive(true);
		particle_->SetPosition(object_.lock()->worldTransform.translate);
	}

	// 当たったら消す
	if (isHit_) {
		object_.lock()->isAlive = false;
	}

	// 発射間隔つける
	if (GetWorldPosition().z - player_->GetWorldPosition().z <= 5000.0f) {
		// 発射タイマーをデクリメント
		--fireTimer_;
		if (fireTimer_ <= 0) {
			// 弾を発射
			Fire();
			// 発射タイマーの初期化
			fireTimer_ = kFireInterval_;
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
		IBullet* baseBullet = bulletObjectPool_->GetBullet("enemy");
		// 取ってこれたかチェックする
		if (baseBullet) {
			EnemyBullet* bullet = dynamic_cast<EnemyBullet*>(baseBullet);
			bullet->Initialize();
			bullet->SetTarget(player_);
			bullet->SetPosition(GetWorldPosition());
			bullet->SetVelocity(velocity);
			bullet->SetIsActive(true);
		}
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
	worldPos.x = object_.lock()->worldTransform.matWorld.m[3][0];
	worldPos.y = object_.lock()->worldTransform.matWorld.m[3][1];
	worldPos.z = object_.lock()->worldTransform.matWorld.m[3][2];

	return worldPos;
}

