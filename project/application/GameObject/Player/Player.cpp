/**
* @file Player.cpp
* @brief プレイヤー（自機）の生成、挙動
* @author 仁平 琉乃
*/

#include "Player.h"
#include "application/GameObject/LockOn/LockOn.h"

void Player::Initialize(uint32_t texHandle)
{
	object_ = std::make_unique<Object3DPlacer>();
	object_->Initialize();
	object_->SetModel("Player/Jet.obj");
	worldTransform_.Initialize(); ;
	object_->SetWorldTransform(worldTransform_);
	object_->SetTexHandle(texHandle);
	object_->SetColor({ 0.0f,0.0f,1.0f,1.0f });
	texHandleBullet_ = TextureManager::Load("resources/TempTexture/white.png"); // bulletの画像

	particle_ = std::make_unique<PlayerParticle>();
	particle_->Initialize();
	explosionParticle_ = std::make_unique<ExplosionParticle>();
	explosionParticle_->Initialize();

	SetCollosionAttribute(kCollisionAttributePlayer);
	SetCollisionMask(kCollisionAttributeEnemy); // 当たる対象

	// 体力
	hp_ = kMaxHp_;
	// 死亡フラグ
	isDead_ = false;
	deadTimer_ = 60.0f;
}

void Player::Update()
{
	Move(); // 移動
	Attack(); // 攻撃
	UpdateBullet(); // 弾の更新
	Rotate(); // 回転
	particle_->SetPosition(GetWorldPosition());
	particle_->SetAreaMax({ GetWorldPosition().x + 0.1f, GetWorldPosition().y + 0.1f,GetWorldPosition().z - 0.5f });
	particle_->SetAreaMin({ GetWorldPosition().x - 0.1f, GetWorldPosition().y - 0.1f,GetWorldPosition().z - 0.7f });
	particle_->Update();
	worldTransform_.UpdateMatrix();

	// 敵の攻撃を食らったら
	const uint32_t kDamage = 1; // 敵からの攻撃ダメージ
	if (isHitEnemyFire_) {
		hp_ -= kDamage;
		isHitEnemyFire_ = false;
	}

	if (hp_ <= 2) {
		isDead_ = true;
	}

	if (isDead_) {
		--deadTimer_;
	}

	if (deadTimer_ <= 30.0f) {
		explosionParticle_->Update();
	}
	else {
		explosionParticle_->SetPosition(GetWorldPosition());
	}

#ifdef _DEBUG
	ImGui::Begin("PlayerRotate");
	ImGui::Text("rotate [x: %.3f ] [y: %.3f] [z: %.3f]", worldTransform_.rotate.x,
		worldTransform_.rotate.y, worldTransform_.rotate.z);
	ImGui::End();

	ImGui::Begin("playerHp");
	ImGui::Text("hp %d", hp_);
	ImGui::End();

#endif
}

void Player::Draw(Camera& camera)
{
	// 生きてるときだけ
	if (!isDead_) {
		object_->Draw(camera);

		// 弾の描画
		for (const auto& bullet : bullets_) {
			bullet->Draw(camera);
		}
	}

	if (deadTimer_ >= 30.0f) {
		particle_->Draw(camera);
	}
	

	// UIの描画
	DrawUI();

	// 死んだときの爆発
	if (deadTimer_ <= 30.0f) {
		explosionParticle_->Draw(camera);
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

}

void Player::Attack()
{

	XINPUT_STATE joyState{};

	// 処理
	if (Input::GetInstance()->PressedKey(DIK_SPACE)) {
		// 弾の速度
		const float kBulletSpeed = 3.0f;
		Vector3 velocity = { 0,0,kBulletSpeed };
		// 弾を生成し、初期化
		std::unique_ptr<PlayerBullet> bullet = std::make_unique<PlayerBullet>();
		bullet->Initialize(texHandleBullet_);
		bullet->SetVelocity(velocity);
		bullet->SetPosition(GetWorldPosition());
		bullets_.push_back(std::move(bullet));
	}

	// ゲームパッドの状態を得る変数(XINPUT)
	if (Input::GetInstance()->GetJoystickState(joyState)) {

		if (Input::GetInstance()->PressedButton(XINPUT_GAMEPAD_RIGHT_SHOULDER)) {
			// 弾の速度
			const float kBulletSpeed = 1.0f;
			Vector3 velocity = { 0,0,kBulletSpeed };
			std::list<Vector3> lockOnVelocity;
			// 自機から照準オブジェクトのベクトル
			Vector3 WorldPos = GetWorldPosition();

			if (!lockOn_->GetTarget().empty()) {
				std::list<const Enemy*> targets = lockOn_->GetTarget(); // コピーしておく
				for (std::list<const Enemy*>::iterator targetItr = targets.begin(); targetItr != targets.end(); ++targetItr) {
					// ロックオン対象がいるかつ生きてたら対象に向かって弾を撃つ
					if ((*targetItr) && !(*targetItr)->GetIsDead()) {
						// レティクルのworld座標にターゲットのworld座標を入れる
						Vector3 targetWorldPos = (*targetItr)->GetWorldPosition();
						Vector3 diff = targetWorldPos - WorldPos;
						diff = Normalize(diff);
						velocity = Normalize(velocity);
						velocity = Multiply(kBulletSpeed, diff);

						// プレイヤーの向きに速度を合わせる
						//velocity = TransformNormal(velocity, worldTransform_.matWorld);

						std::unique_ptr<PlayerBullet> bullet = std::make_unique<PlayerBullet>();
						bullet->SetLockOn(lockOn_);
						bullet->Initialize(texHandleBullet_);
						bullet->SetPosition(WorldPos);
						bullet->SetVelocity(velocity);
						bullets_.push_back(std::move(bullet));
					}
				}
			}
			else {
				Vector3 reticleWorldPos = lockOn_->GetWorldPosition3DReticle();

				velocity = reticleWorldPos - WorldPos;
				velocity = Normalize(velocity);
				velocity = kBulletSpeed * velocity;
				// プレイヤーの向きに速度を合わせる
				//velocity = TransformNormal(velocity, worldTransform_.matWorld);
				// 弾を生成し、初期化
				std::unique_ptr<PlayerBullet> bullet = std::make_unique<PlayerBullet>();
				bullet->Initialize(texHandleBullet_);
				bullet->SetPosition(WorldPos);
				bullet->SetVelocity(velocity);
				bullets_.push_back(std::move(bullet));
			}

		}

	}

}

void Player::UpdateBullet()
{
	// 弾更新
	for (const auto& bullet : bullets_) {
		bullet->Update();
	}

	// デスフラグが立ったら要素を削除
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		if (bullet->GetIsDead()) {
			return true;
		}
		return false;
		});

}

void Player::OnCollision()
{
	isHitEnemyFire_ = true; // 敵の攻撃が当たった
}

void Player::DrawUI()
{

}

void Player::Rotate()
{
	
	Vector3 end = lockOn_->GetWorldTransform();
	Vector3 start = worldTransform_.translate;

	Vector3 diff = end - start;

	diff = Normalize(diff);

	Vector3 velocity = diff;

	float t = 0.0f;

	// 引数で受け取った速度をメンバ変数に代入
	velocity_ = SLerp(velocity, worldTransform_.translate, t);

	velocity_.x *= 0.1f;
	velocity_.y *= 0.1f;
	velocity_.z *= 0.1f;

	// Y軸周り角度（Θy）
	worldTransform_.rotate.y = std::atan2(velocity_.x, velocity_.z);

	float velocityXZ = sqrt((velocity_.x * velocity_.x) + (velocity_.z * velocity_.z));
	worldTransform_.rotate.x = std::atan2(-velocity_.y, velocityXZ);

	// 座標移動(ベクトルの加算)
	worldTransform_.translate.x += velocity.x * 0.4f;
	worldTransform_.translate.y += velocity.y * 0.4f;
	worldTransform_.translate.z += velocity.z * 0.4f;

	particle_->SetVelocityXY({ -velocity.x,-velocity.y });

}

Vector3 Player::GetWorldPosition() const
{
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.matWorld.m[3][0];
	worldPos.y = worldTransform_.matWorld.m[3][1];
	worldPos.z = worldTransform_.matWorld.m[3][2];

	return worldPos;
}