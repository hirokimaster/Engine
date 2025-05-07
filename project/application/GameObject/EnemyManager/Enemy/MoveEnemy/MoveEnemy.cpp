/**
* @file Enemy.cpp
* @brief 敵の生成、挙動
* @author 仁平 琉乃
*/

#include "application/GameObject/Player/Player.h"
#include "MoveEnemy.h"

void MoveEnemy::Initialize()
{
	// object共通の初期化
	BaseInstancingObject::Initialize("Enemy/enemy.obj", "TempTexture/noise0.png", ColliderType::Sphere);
	object_.lock()->color = (Vector4(0.0f, 0.0f, 0.0f, 1.0f));
	object_.lock()->worldTransform.scale = { 10.0f,10.0f,10.0f };
	//object_->SetRotate({ 0.0f,std::numbers::pi_v<float>,0.0f });
	
	// 調整項目追加
	AddAdjustmentVariables();
	// 調整項目適用
	ApplyAdjustmentVariables();

	// 最初の状態
	phaseState_ = std::make_unique<EnemyStateSortie>();

	// 当たり判定の属性設定
	collider_->SetCollosionAttribute(kCollisionAttributeEnemy);
	collider_->SetCollisionMask(kCollisionAttributePlayer); // 当たる対象
	collider_->SetRadious(8.0f);
	
	velocity_ = { 0.0f,0.0f,1.0f };

	// パーティクル
	particleManager_ = ParticleManager::GetInstance();

	bulletSpeed_ = 10.0f;
}

void MoveEnemy::Update()
{
	BaseInstancingObject::Update(); // object共通の更新処理
	collider_->SetWorldPosition(GetWorldPosition());
	phaseState_->SetPlayer(player_);
	phaseState_->Update(this); // 状態ごとの更新処理

	OnCollision(); // 当たったら

	// パーティクル
	if (isHit_ && !isExploded_) {
		particle_ = particleManager_->GetParticle("explosion", "Player/smoke.png");
		isExploded_ = true;
	}

	// particleの位置
	if (particle_) {
		particle_->SetIsActive(true);
		particle_->SetPosition(object_.lock()->worldTransform.translate);
	}

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
}

void MoveEnemy::Draw(const Camera& camera)
{
	// 出撃するまで出さない
	if (isSortie_ && !isHit_) {
		camera;
	}
}

void MoveEnemy::OnCollision()
{
	if (collider_->OnCollision()) {
		isHit_ = true;
		// 撃破数を足す
		player_->AddDestroyCount();
	}
}

void MoveEnemy::Fire()
{
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

void MoveEnemy::AddAdjustmentVariables()
{
	AdjustmentVariables* variables = AdjustmentVariables::GetInstance();
	const char* groupName = "Enemy";
	// グループを追加
	variables->CreateGroup(groupName);
	// アイテム追加
	variables->AddItem(groupName, "bulletSpeed", bulletSpeed_);
}

void MoveEnemy::ApplyAdjustmentVariables()
{
	AdjustmentVariables* variables = AdjustmentVariables::GetInstance();
	const char* groupName = "Enemy";
	bulletSpeed_ = variables->GetValue<float>(groupName, "bulletSpeed");
}

void MoveEnemy::ChangeState(std::unique_ptr<BasePhaseStateEnemy> newState)
{
	phaseState_ = std::move(newState);
}

void MoveEnemy::Move()
{
	// 移動
	Vector3 move{};
	move = object_.lock()->worldTransform.translate + velocity_;
	SetPosition(move);
}

Vector3 MoveEnemy::GetWorldPosition() const
{
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = object_.lock()->worldTransform.matWorld.m[3][0];
	worldPos.y = object_.lock()->worldTransform.matWorld.m[3][1];
	worldPos.z = object_.lock()->worldTransform.matWorld.m[3][2];

	return worldPos;
}
