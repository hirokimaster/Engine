/**
* @file Enemy.cpp
* @brief 敵の生成、挙動
* @author 仁平 琉乃
*/

#include "Enemy.h"
#include "application/GameObject/Player/Player.h"

void Enemy::Initialize(uint32_t texHandle)
{
	object_ = std::make_unique<Object3DPlacer>();
	object_->Initialize();
	object_->SetModel("Enemy/cube.obj");
	worldTransform_.Initialize();
	object_->SetWorldTransform(worldTransform_);
	object_->SetTexHandle(texHandle);

	texHandleBullet_ = TextureManager::Load("resources/TempTexture/white.png"); // bulletの画像
	object_->SetColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));

	deadParticle_ = std::make_unique<ExplosionParticle>();
	deadParticle_->Initialize();
	bulletSpeed_ = 1.0f;


	// 最初の状態
	phaseState_ = std::make_unique<EnemyStateSortie>();

	// 当たり判定の属性設定
	SetCollosionAttribute(kCollisionAttributeEnemy);
	SetCollisionMask(kCollisionAttributePlayerBullet); // 当たる対象
}

void Enemy::Update()
{
	worldTransform_.UpdateMatrix();
	phaseState_->SetPlayer(player_);

	phaseState_->Update(this); // 状態ごとの更新処理

	if (!isParticle_) {
		BulletUpdate(); // 弾の更新処理
	}
	
	// 時間で消滅
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	if (isParticle_) {
		deadParticle_->Update();
	}
	else {
		// 死んだときのparticle
		deadParticle_->SetPosition(GetWorldPosition());
	}
	
	if (isParticle_) {
		--particleTimer_;
	}

	if (particleTimer_ <= 0) {
		particleTimer_ = 0;
		isParticle_ = false;
		isDead_ = true;
	}

	if (particleTimer_ > 120) {
		deadParticle_->SetIsExplosion(true);
	}

#ifdef _DEBUG

	ImGui::Begin("Enemy");
	ImGui::DragFloat3("translate", &worldTransform_.translate.x, 0.1f, -100.0f, 100.0f);
	ImGui::End();

#endif
}

void Enemy::Draw(Camera& camera)
{
	// 出撃するまで出さない
	if (isSortie_ && !isParticle_) {
		object_->Draw(camera);
	}

	if (isParticle_) {
		deadParticle_->Draw(camera);
	}

	if (!isParticle_) {
		// 弾の描画
		for (const auto& bullet : bullets_) {
			bullet->Draw(camera);
		}
	}

}

void Enemy::OnCollision()
{
	isParticle_ = true;
}

void Enemy::Fire()
{
	Vector3 playerWorldPos = player_->GetWorldPosition(); // 自キャラのワールド座標を取得
	Vector3 enemyWorldPos = GetWorldPosition(); // 敵キャラのワールド座標を取得
	Vector3 diff = Subtract(playerWorldPos, enemyWorldPos); // 差分ベクトルを求める
	Normalize(diff); // 正規化
	Vector3 velocity = Multiply(bulletSpeed_, diff); // ベクトルの速度

	// 弾を生成して初期化
	std::unique_ptr<EnemyBullet> bullet = std::make_unique<EnemyBullet>();
	bullet->Initialize(texHandleBullet_);
	bullet->SetPosition(GetWorldPosition());
	bullet->SetVelocity(velocity);
	// 弾をセット
	bullets_.push_back(std::move(bullet));
}

void Enemy::BulletUpdate()
{

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

void Enemy::AddAdjustmentVariables()
{
	AdjustmentVariables* variables = AdjustmentVariables::GetInstance();
	const char* groupName = "Enemy";
	// グループを追加
	variables->CreateGroup(groupName);
	// アイテム追加
	variables->AddItem(groupName, "bulletSpeed", bulletSpeed_);
}

void Enemy::ApplyAdjustmentVariables()
{
	AdjustmentVariables* variables = AdjustmentVariables::GetInstance();
	const char* groupName = "Enemy";
	bulletSpeed_ = variables->GetValue<float>(groupName, "bulletSpeed");
}

void Enemy::ChangeState(std::unique_ptr<IPhaseStateEnemy> newState)
{
	phaseState_ = std::move(newState);
}

void Enemy::Move()
{
	// 移動
	worldTransform_.translate = worldTransform_.translate + velocity_;
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
