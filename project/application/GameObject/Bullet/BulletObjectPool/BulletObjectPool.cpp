/**
* @file BulletObjectPool.cpp
* @brief 弾のオブジェクトプール
* @author 仁平 琉乃
*/

#include "BulletObjectPool.h"

void BulletObjectPool::Initialize()
{
	// 初期化時にあらかじめ最大の半分作っておく
	for (uint32_t i = 0; i < kPoolSize / 2; ++i) {
		Create("player");
		Create("enemy");
	}

	trajectory_ = std::make_unique<BulletTrajectory>();
	trajectory_->Initialize();
}

void BulletObjectPool::Update()
{
	// 弾の更新
	for (auto& bullet : bullets_) {
		// アクティブ状態で生きてる弾のみ更新
		if (bullet->GetIsActive() && !bullet->GetIsDead()) {
			bullet->Update();

			trajectory_->AddTrailPoint(bullet->GetWorldPosition());
		}

		// 弾が死んだらキューに戻す
		if (bullet->GetIsDead()) {
		
			if (bullet->GetBulletType() == BulletType::Player) {
				Push("player", bullet.get());
			}
			else if (bullet->GetBulletType() == BulletType::Enemy) {
				Push("enemy", bullet.get());
			}
		}
	}

	trajectory_->Update();
}

IBullet* BulletObjectPool::GetBullet(const std::string& name)
{

	// 空のときは新しく作る
	if (pool_[name].empty()) {
		// 最大数を超えてたら
		if (bullets_.size() >= kPoolSize) {
			return nullptr;
		}
		Create(name);
	}

	// キューから取り出す
	IBullet* bullet = pool_[name].front();
	pool_[name].pop();

	return bullet;
}

void BulletObjectPool::Create(const std::string& name)
{
	// 弾のインスタンス生成
	std::unique_ptr<IBullet> bullet;
	if (name == "player") {
		bullet = std::make_unique<PlayerBullet>();
		bullet->Initialize();
	}
	else if (name == "enemy") {
		bullet = std::make_unique<EnemyBullet>();
		bullet->Initialize();
	}

	// キューに入れる
	IBullet* ptr = bullet.get();
	bullets_.push_back(std::move(bullet));
	pool_[name].push(ptr);
}

void BulletObjectPool::Push(const std::string& name, IBullet* bullet)
{
	// bulletがnullだったら通っちゃだめ
	if (!bullet) return;

	// アクティブ状態解除
	bullet->SetIsActive(false);
	bullet->SetIsDead(false);
	// 消滅時間リセット
	bullet->ResetDeathTimer();

	// キューに戻す
	pool_[name].push(bullet);
}
