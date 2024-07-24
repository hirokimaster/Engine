#include "Player.h"
#include "application/GameObject/LockOn/LockOn.h"

void Player::Initialize(uint32_t texHandle)
{
	object_ = std::make_unique<Object3DPlacer>();
	object_->Initialize();
	object_->SetModel("cube.obj");
	worldTransform_.Initialize(); ;
	object_->SetWorldTransform(worldTransform_);
	object_->SetTexHandle(texHandle);
	texHandleBullet_ = TextureManager::Load("resources/uvChecker.png"); // bulletの画像

	SetCollosionAttribute(kCollisionAttributePlayer);
	SetCollisionMask(kCollisionAttributeEnemy); // 当たる対象
}

void Player::Update()
{
	Move(); // 移動
	Attack(); // 攻撃
	UpdateBullet(); // 弾の更新
	worldTransform_.UpdateMatrix();
}

void Player::Draw(Camera& camera)
{
	object_->Draw(camera);

	// 弾の描画
	for (bulletsItr_ = bullets_.begin();
		bulletsItr_ != bullets_.end(); ++bulletsItr_) {
		(*bulletsItr_)->Draw(camera);
	}

	// UIの描画
	DrawUI();
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

	// ゲームパッドの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;

	// ゲームパッド状態取得
	if (Input::GetInstance()->GetJoystickState(joyState)) {
		move.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * kMoveSpeed_;
		move.y += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * kMoveSpeed_;
	}

	// 移動
	worldTransform_.translate = worldTransform_.translate + move;
}

void Player::Attack()
{

	XINPUT_STATE joyState{};

	// 処理
	if (Input::GetInstance()->PressedKey(DIK_SPACE)) {
		// 弾の速度
		const float kBulletSpeed = 1.0f;
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

			if (lockOn_->GetIsLockOnMode()) {
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
	for (bulletsItr_ = bullets_.begin();
		bulletsItr_ != bullets_.end(); ++bulletsItr_) {
		(*bulletsItr_)->Update();
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

}

void Player::DrawUI()
{

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