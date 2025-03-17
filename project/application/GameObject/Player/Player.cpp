/**
* @file Player.cpp
* @brief プレイヤー（自機）の生成、挙動
* @author 仁平 琉乃
*/

#include "Player.h"
#include "application/GameObject/LockOn/LockOn.h"

void Player::Initialize()
{
	object_ = std::make_unique<Object3DPlacer>();
	object_->Initialize();
	object_->SetModel("Player/player.obj");
	object_->SetPosition({ 0,-20.0f,0 });
	object_->SetTexHandle(TextureManager::GetTexHandle("TempTexture/white.png"));

	// collider
	collider_ = std::make_unique<Collider>();
	collider_->SetCollosionAttribute(kCollisionAttributePlayer); // 自分の属性
	collider_->SetCollisionMask(kCollisionAttributeEnemyBullet); // 当たる対象
	collider_->SetType(ColliderType::Sphere); // どの形状でとるか

	// 調整項目
	AddAdjustmentVariables();


	// 死亡フラグ
	isDead_ = false;
	deadTimer_ = 60.0f;

	// 発射タイマー
	attackTimer_ = 0.0f;

	// UI
	spriteAttack_.reset(Sprite::Create(TextureManager::GetTexHandle("UI/RB.png"), { 1000.0f , 500.0f }));
	spriteAttack_->SetScale({ 2.0f,2.0f });
	spriteMove_.reset(Sprite::Create(TextureManager::GetTexHandle("UI/L.png"), { 240.0f,500.0f }));
	spriteMove_->SetScale({ 2.0f,2.0f });
}

void Player::Update()
{

	Move(); // 移動
	Rotate(); // 回転

	// 発射間隔
	const float kAttackInterval = 0.2f;
	attackTimer_ += 1.0f / 60.0f;

	// Rトリガーを押していたら攻撃
	if (Input::GetInstance()->PushButton(XINPUT_GAMEPAD_RIGHT_SHOULDER)) {
		spriteAttack_->SetTexHandle(TextureManager::GetTexHandle("UI/RB2.png"));

		// 発射間隔をつける
		if (attackTimer_ >= kAttackInterval) {
			Attack(); // 攻撃
			attackTimer_ = 0.0f;
		}

	}
	else {
		spriteAttack_->SetTexHandle(TextureManager::GetTexHandle("UI/RB.png"));
	}

	UpdateBullet(); // 弾の更新
	object_->Update();
	collider_->SetWorldPosition(GetWorldPosition()); // colliderにワールド座標を送る
	OnCollision(); // 当たったら
	// ダメージ
	IncurDamage();

	// imgui
	DebugDraw();

	// 撃破カウントが一定数いったらマルチロックオンにする(まだできてない)
	if (destroyCount_ >= 5) {
		//lockOn_->SetIsLockOnMode(true);
		destroyCount_ = 0;
	}

	ApplyAdjustmentVariables();
}

void Player::Draw(const Camera& camera)
{
	object_->Draw(camera);

	// 弾の描画
	for (const auto& bullet : bullets_) {
		bullet->Draw(camera);
	}
}

void Player::Move()
{
	XINPUT_STATE joyState{};
	Vector3 move{};

	// ジョイスティック状態取得
	if (Input::GetInstance()->GetJoystickState(joyState)) {
		move.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX;
		move.y += (float)joyState.Gamepad.sThumbLY / SHRT_MAX;
	}

	Vector3 position = object_->GetWorldTransform().translate + move;
	position.z += moveSpeed_;
	// 移動範囲を制限
	position.x = std::clamp(position.x, moveMinLimit_.x, moveMaxLimit_.x);
	position.y = std::clamp(position.y, moveMinLimit_.y, moveMaxLimit_.y);
	object_->SetPosition(position);
}

void Player::Attack()
{
	// 弾の速度
	Vector3 velocity = { 0,0,bulletSpeed_ };
	std::list<Vector3> lockOnVelocity;
	// 自機から照準オブジェクトのベクトル
	Vector3 WorldPos = GetWorldPosition();
	// ロックオンしているターゲットがいたら
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
				bullet->Initialize(TextureManager::GetTexHandle("TempTexture/white.png"));
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
		bullet->Initialize(TextureManager::GetTexHandle("TempTexture/white.png"));
		const float kDistanceZ = 5.0f;
		Vector3 position = WorldPos;
		position.z = position.z + kDistanceZ;
		bullet->SetPosition(position);
		bullet->SetVelocity(velocity);
		bullets_.push_back(std::move(bullet));
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

	if (collider_->OnCollision()) {
		isHitEnemyFire_ = true; // 敵の攻撃が当たった
	}
}

void Player::DrawUI()
{
	spriteAttack_->Draw();
	spriteMove_->Draw();
}

void Player::Rotate()
{
	Vector3 rotate = { 0, 0, 0 };
	const float kRotateLimitZ = 0.7f;
	const float kRotateLimitX = 0.15f;
	const float kLerpFactor = 0.5f;
	
	// ゲームパッドの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;

	// ゲームパッド状態取得
	if (Input::GetInstance()->GetJoystickState(joyState)) {
		// ゲームパッドの入力から回転速度を計算
		rotate.z += (float)joyState.Gamepad.sThumbLX / SHRT_MAX;
		rotate.x += (float)joyState.Gamepad.sThumbLY / SHRT_MAX;
	}

	Vector3 rotateVelo{};
	// 回転を適用
	rotateVelo.z = std::lerp(object_->GetWorldTransform().rotate.z, object_->GetWorldTransform().rotate.z - rotate.z, kLerpFactor);
	rotateVelo.x = std::lerp(object_->GetWorldTransform().rotate.x, object_->GetWorldTransform().rotate.x - rotate.x, kLerpFactor);

	// z軸に制限をかける
	rotateVelo.z = std::clamp(rotateVelo.z, -kRotateLimitZ, kRotateLimitZ);

	// x軸に制限をかける
	rotateVelo.x = std::clamp(rotateVelo.x, -kRotateLimitX, kRotateLimitX);

	// y軸は回転させないので0にしとく
	rotateVelo.y = 0.0f;

	object_->SetRotate(rotateVelo);
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
	ImGui::Begin("player");
	ImGui::Text("position = %f : %f : %f", object_->GetWorldTransform().translate.x,
		object_->GetWorldTransform().translate.y, object_->GetWorldTransform().translate.z);
	ImGui::End();

	ImGui::Begin("PlayerRotate");
	ImGui::Text("rotate [x: %.3f ] [y: %.3f] [z: %.3f]", object_->GetWorldTransform().rotate.x,
		object_->GetWorldTransform().rotate.y, object_->GetWorldTransform().rotate.z);
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
	variables->AddItem(groupName, "rotate", rotate_);
	variables->AddItem(groupName, "hp", hp_);
	variables->AddItem(groupName, "bulletSpeed", bulletSpeed_);
	variables->AddItem(groupName, "moveMinLimit", moveMinLimit_);
	variables->AddItem(groupName, "moveMaxLimit", moveMaxLimit_);
	variables->AddItem(groupName, "rotateSpeed", rotateSpeed_);
	variables->AddItem(groupName, "rotateLerpFactor", rotateLerpFactor_);
}

void Player::ApplyAdjustmentVariables()
{
	AdjustmentVariables* variables = AdjustmentVariables::GetInstance();
	const char* groupName = "Player";
	moveSpeed_ = variables->GetValue<float>(groupName, "moveSpeed");
	rotate_ = variables->GetValue<Vector3>(groupName, "rotate");
	object_->SetRotate(rotate_);
	bulletSpeed_ = variables->GetValue<float>(groupName, "bulletSpeed");
	hp_ = variables->GetValue<int32_t>(groupName, "hp");
	moveMinLimit_ = variables->GetValue<Vector3>(groupName, "moveMinLimit");
	moveMaxLimit_ = variables->GetValue<Vector3>(groupName, "moveMaxLimit");
	rotateSpeed_ = variables->GetValue<float>(groupName, "rotateSpeed");
	rotateLerpFactor_ = variables->GetValue<float>(groupName, "rotateLerpFactor");
}

Vector3 Player::GetWorldPosition() const
{
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = object_->GetWorldTransform().matWorld.m[3][0];
	worldPos.y = object_->GetWorldTransform().matWorld.m[3][1];
	worldPos.z = object_->GetWorldTransform().matWorld.m[3][2];

	return worldPos;
}