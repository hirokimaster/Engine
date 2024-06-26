#include "Player.h"

void Player::Initialize(Object3DPlacer* object, uint32_t texHandle, const std::string& model)
{
	BaseObject::Initialize(object, texHandle, model); // 共通処理初期化
	texHandleBullet_ = TextureManager::Load("resources/uvChecker.png"); // bulletの画像
}

void Player::Update()
{
	Move(); // 移動
	Attack(); // 攻撃
	UpdateBullet(); // 弾の更新
	BaseObject::Update(); // 共通の更新処理
}

void Player::Draw(Camera& camera)
{
	BaseObject::Draw(camera); // 共通の描画処理

	// 弾の描画
	for (bulletsItr_ = bullets_.begin();
		bulletsItr_ != bullets_.end(); ++bulletsItr_) {
		(*bulletsItr_)->Draw(camera);
	}
}

void Player::Move()
{
	Vector3 move{ 0,0,0 }; // 移動ベクトル

	if (Input::GetInstance()->PushKey(DIK_A)) {
		move.x -= kMoveSpeed_;
	}
	else if (Input::GetInstance()->PushKey(DIK_D)) {
		move.x += kMoveSpeed_;
	}

	if (Input::GetInstance()->PushKey(DIK_W)) {
		move.y += kMoveSpeed_;
	}
	else if (Input::GetInstance()->PushKey(DIK_S)) {
		move.y -= kMoveSpeed_;
	}

	// 移動
	worldTransform_.translate = worldTransform_.translate + move;
}

void Player::Attack()
{

	// 弾の速度
	const float kBulletSpeed = 1.0f;
	Vector3 velocity = { 0,0,kBulletSpeed };

	// 処理
	if (Input::GetInstance()->PressedKey(DIK_SPACE)) {
		// 弾を生成し、初期化
		std::unique_ptr<PlayerBullet> bullet = std::make_unique<PlayerBullet>();
		bulletObject_ = std::make_unique<Object3DPlacer>();
		bullet->Initialize(bulletObject_.get(), texHandleBullet_, "cube.obj");
		bullet->SetVelocity(velocity);
		bullet->SetPosition(worldTransform_.translate);
		bullets_.push_back(std::move(bullet));
	}

}

void Player::UpdateBullet()
{
	// 弾更新
	for (bulletsItr_ = bullets_.begin();
		bulletsItr_ != bullets_.end(); ++bulletsItr_) {
		(*bulletsItr_)->Update();
	}

}
