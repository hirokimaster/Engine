#include "FixedEnemy.h"
#include "application/GameObject/Player/Player.h"

void FixedEnemy::Initialize()
{
	// object共通の初期化
	BaseObject::Initialize("LevelEditorObj/fixedEnemy.obj", "TempTexture/noise0.png", ColliderType::Sphere);
	object_->SetColor({ 1.0f,0.0f,0.0f,1.0f });
	// コライダーの属性設定
	collider_->SetCollosionAttribute(kCollisionAttributeEnemy);	  // 自分の属性
	collider_->SetCollisionMask(kCollisionAttributePlayerBullet); // 当たる対象
	collider_->SetRadious(8.0f); // コライダーのサイズ 
	// パーティクル
	particleManager_ = ParticleManager::GetInstance();
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
	}
	
	// particleの位置
	if (particle_) {
		particle_->SetIsActive(true);
		particle_->SetPosition(object_->GetWorldTransform().translate);
	}
	
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
	}
	
}

void FixedEnemy::Fire()
{
	// playerがいなかったらそもそも撃つ対象がいない
	if (player_) {
		Vector3 playerWorldPos = player_->GetWorldPosition(); // 自キャラのワールド座標を取得
		Vector3 enemyWorldPos = GetWorldPosition(); // 敵キャラのワールド座標を取得
		Vector3 diff = Subtract(playerWorldPos, enemyWorldPos); // 差分ベクトルを求める
		Normalize(diff); // 正規化
		Vector3 velocity = Multiply(bulletSpeed_, diff); // ベクトルの速度

		// 弾を生成して初期化
		// プールから取ってくる
		IBullet* baseBullet = bulletObjectPool_->GetBullet("enemy");
		// 取ってこれたかチェックする
		if (baseBullet) {
			EnemyBullet* bullet = dynamic_cast<EnemyBullet*>(baseBullet);
			bullet->Initialize();
			bullet->SetPosition(GetWorldPosition());
			bullet->SetVelocity(velocity);
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
	worldPos.x = object_->GetWorldTransform().matWorld.m[3][0];
	worldPos.y = object_->GetWorldTransform().matWorld.m[3][1];
	worldPos.z = object_->GetWorldTransform().matWorld.m[3][2];

	return worldPos;
}

