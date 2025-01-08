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
	worldTransform_.Initialize();
	worldTransform_.translate.y = 20.0f;
	object_->SetWorldTransform(worldTransform_);
	object_->SetTexHandle(texHandle);
	texHandleBullet_ = TextureManager::Load("resources/TempTexture/white.png"); // bulletの画像

	SetCollosionAttribute(kCollisionAttributePlayer);
	SetCollisionMask(kCollisionAttributeEnemyBullet); // 当たる対象

	AddAdjustmentVariables();
	ApplyAdjustmentVariables();

	// 死亡フラグ
	isDead_ = false;
	deadTimer_ = 60.0f;
}

void Player::Update()
{
	Move(); // 移動
	Rotate(); // 回転
	Attack(); // 攻撃
	UpdateBullet(); // 弾の更新
	worldTransform_.UpdateMatrix();

	// ダメージ
	IncurDamage();

	// imgui
	DebugDraw();

	// 撃破カウントが一定数いったらマルチロックオンにする
	if (destroyCount_ >= 5) {
		//lockOn_->SetIsLockOnMode(true);
		destroyCount_ = 0;
	}
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

	// UIの描画
	DrawUI();
}

void Player::Move()
{

	Vector3 move = { 0,0,0 }; // 移動ベクトル
	const float kMoveSpeed = 0.2f;

	// ゲームパッドの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;

	// ゲームパッド状態取得
	if (Input::GetInstance()->GetJoystickState(joyState)) {
		move.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * kMoveSpeed;
		move.y += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * kMoveSpeed;
	}

	worldTransform_.translate = worldTransform_.translate + move;

	worldTransform_.translate.z = worldTransform_.translate.z + moveSpeed_;
}

void Player::Attack()
{

	if (Input::GetInstance()->PressedButton(XINPUT_GAMEPAD_RIGHT_SHOULDER)) {
		// 弾の速度
		Vector3 velocity = { 0,0,bulletSpeed_ };
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
					velocity = Multiply(bulletSpeed_, diff);
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
			velocity = bulletSpeed_ * velocity;
			// 弾を生成し、初期化
			std::unique_ptr<PlayerBullet> bullet = std::make_unique<PlayerBullet>();
			bullet->Initialize(texHandleBullet_);
			const float kDistanceZ = 5.0f;
			Vector3 position = WorldPos;
			position.z = position.z + kDistanceZ;
			bullet->SetPosition(position);
			bullet->SetVelocity(velocity);
			bullets_.push_back(std::move(bullet));
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
	Vector3 rotate = { 0, 0, 0 };
	const float kRotateSpeedZ = 0.07f;
	const float kRotateSpeedX = 0.07f;
	const float kRotateLimitZ = 0.7f;
	const float kRotateLimitX = 0.15f;
	const float kLerpFactor = 0.1f;
	const float kReturnLerpFactor = 0.05f;

	// ゲームパッドの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;

	// ゲームパッド状態取得
	if (Input::GetInstance()->GetJoystickState(joyState)) {
		// ゲームパッドの入力から回転速度を計算
		rotate.z = (float)joyState.Gamepad.sThumbLX / SHRT_MAX * kRotateSpeedZ;
		rotate.x = (float)joyState.Gamepad.sThumbLY / SHRT_MAX * kRotateSpeedX;
	}

	// 回転を適用（補間を加える）
	worldTransform_.rotate.z = std::lerp(worldTransform_.rotate.z, worldTransform_.rotate.z - rotate.z, kLerpFactor);
	worldTransform_.rotate.x = std::lerp(worldTransform_.rotate.x, worldTransform_.rotate.x - rotate.x, kLerpFactor);

	// z軸に制限をかける
	worldTransform_.rotate.z = std::clamp(worldTransform_.rotate.z, -kRotateLimitZ, kRotateLimitZ);

	// x軸に制限をかける
	worldTransform_.rotate.x = std::clamp(worldTransform_.rotate.x, -kRotateLimitX, kRotateLimitX);

	// y軸は回転させないので0にしとく
	worldTransform_.rotate.y = 0.0f;

	// 操作がない場合は徐々に0に戻す
	if (std::abs(rotate.z) < 0.005f) {
		worldTransform_.rotate.z = std::lerp(worldTransform_.rotate.z, 0.0f, kReturnLerpFactor);
	}
	if (std::abs(rotate.x) < 0.005f) {
		worldTransform_.rotate.x = std::lerp(worldTransform_.rotate.x, 0.0f, kReturnLerpFactor);
	}
}

void Player::IncurDamage()
{
	// 敵の攻撃を食らったら
	const uint32_t kDamage = 1; // 敵からの攻撃ダメージ
	if (isHitEnemyFire_) {
		hp_ -= kDamage;
		isHitEnemyFire_ = false;
	}

	if (hp_ <= 0) {
		isDead_ = true;
	}

	if (isDead_) {
		--deadTimer_;
	}
}

void Player::DebugDraw()
{
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

void Player::AddAdjustmentVariables()
{
	AdjustmentVariables* variables = AdjustmentVariables::GetInstance();
	const char* groupName = "Player";
	// グループを追加
	variables->CreateGroup(groupName);
	// アイテム追加
	variables->AddItem(groupName, "moveSpeed", moveSpeed_);
	variables->AddItem(groupName, "translate", worldTransform_.translate);
	variables->AddItem(groupName, "hp", hp_);
	variables->AddItem(groupName, "bulletSpeed", bulletSpeed_);
}

void Player::ApplyAdjustmentVariables()
{
	AdjustmentVariables* variables = AdjustmentVariables::GetInstance();
	const char* groupName = "Player";
	moveSpeed_ = variables->GetValue<float>(groupName, "moveSpeed");
	worldTransform_.translate = variables->GetValue<Vector3>(groupName, "translate");
	bulletSpeed_ = variables->GetValue<float>(groupName, "bulletSpeed");
	hp_ = variables->GetValue<int32_t>(groupName, "hp");
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