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
	object_->SetModel("LevelEditorObj/enemy.obj");
	object_->SetTexHandle(texHandle);
	object_->SetColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	bulletSpeed_ = 0.03f;

	// 最初の状態
	phaseState_ = std::make_unique<EnemyStateSortie>();

	// 当たり判定の属性設定
	collider_ = std::make_unique<Collider>();
	collider_->SetCollosionAttribute(kCollisionAttributeEnemy);
	collider_->SetCollisionMask(kCollisionAttributePlayerBullet); // 当たる対象
	collider_->SetType(ColliderType::Sphere); // 形状
}

void Enemy::Update()
{
	object_->Update();
	collider_->SetWorldPosition(GetWorldPosition());
	phaseState_->SetPlayer(player_);

	phaseState_->Update(this); // 状態ごとの更新処理

	BulletUpdate(); // 弾の更新処理

	OnCollision(); // 当たったら

	// 時間で消滅
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void Enemy::Draw(Camera& camera)
{
	// 出撃するまで出さない
	if (isSortie_) {
		object_->Draw(camera);
	}

	for (const auto& bullet : bullets_) {
		bullet->Draw(camera);
	}

}

void Enemy::OnCollision()
{
	if (collider_->OnCollision()) {
		isDead_ = true;
		// 撃破数を足す
		player_->AddDestroyCount();
	}
}

void Enemy::Fire()
{
	// playerがいなかったらそもそも撃つ対象がいない
	if (player_) {
		Vector3 playerWorldPos = player_->GetWorldPosition(); // 自キャラのワールド座標を取得
		Vector3 enemyWorldPos = GetWorldPosition(); // 敵キャラのワールド座標を取得
		Vector3 diff = Subtract(playerWorldPos, enemyWorldPos); // 差分ベクトルを求める
		Normalize(diff); // 正規化
		Vector3 velocity = Multiply(bulletSpeed_, diff); // ベクトルの速度

		// 弾を生成して初期化
		std::unique_ptr<EnemyBullet> bullet = std::make_unique<EnemyBullet>();
		bullet->Initialize(TextureManager::GetTexHandle("TempTexture/white.png"));
		bullet->SetPosition(GetWorldPosition());
		bullet->SetVelocity(velocity);
		// 弾をセット
		bullets_.push_back(std::move(bullet));
	}
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

void Enemy::ChangeState(std::unique_ptr<BasePhaseStateEnemy> newState)
{
	phaseState_ = std::move(newState);
}

void Enemy::Move()
{
	// 移動
	Vector3 move{};
	move = object_->GetWorldTransform().translate + velocity_;
	object_->SetPosition(move);
}

Vector3 Enemy::GetWorldPosition() const
{
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = object_->GetWorldTransform().matWorld.m[3][0];
	worldPos.y = object_->GetWorldTransform().matWorld.m[3][1];
	worldPos.z = object_->GetWorldTransform().matWorld.m[3][2];

	return worldPos;
}
