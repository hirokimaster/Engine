/**
* @file Player.cpp
* @brief プレイヤー（自機）の生成、挙動
* @author 仁平 琉乃
*/

#include "Player.h"
#include "application/GameObject/LockOn/LockOn.h"

void Player::Initialize()
{
	// object共通の初期化
	BaseObject::Initialize("Player/player.obj", "TempTexture/white.png", ColliderType::Sphere);
	object_->SetPosition({ 0,40.0f,-1000.0f });
	// 属性設定
	collider_->SetCollosionAttribute(kCollisionAttributePlayer); // 自分の属性
	collider_->SetCollisionMask(kCollisionAttributeEnemyBullet); // 当たる対象
	collider_->SetRadious(2.0f);

	// 調整項目
	AddAdjustmentVariables();

	// 死亡フラグ
	isDead_ = false;
	deadTimer_ = 60.0f;

	// 発射タイマー
	attackTimer_ = 0.0f;

	// particle
	particleManager_ = ParticleManager::GetInstance();

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

	BaseObject::Update(); // object共通の更新処理
	collider_->SetWorldPosition(GetWorldPosition());
	OnCollision(); // 当たったら
	// ダメージ
	IncurDamage();

	// imgui
	DebugDraw();

	// 撃破カウントが一定数いったらマルチロックオンにする(まだできてない)
	if (destroyCount_ >= 5) {
		destroyCount_ = 0;
	}

	ApplyAdjustmentVariables();
}

void Player::Draw(const Camera& camera)
{
	BaseObject::Draw(camera);
}

void Player::Move()
{
	XINPUT_STATE joyState{};
	Vector3 move{};

	// ジョイスティック状態取得
	if (Input::GetInstance()->GetJoystickState(joyState)) {
		move.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * 2.0f;
		move.y += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * 2.0f;
	}

	Vector3 position = object_->GetWorldTransform().translate + move;
	position.z += moveSpeed_;
	object_->SetPosition(position);
	spriteMove_->SetPosition(Vector2((5.0f * move.x) + 240.0f, (5.0f * move.y) + 500.0f));
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
		std::list<const IEnemy*> targets = lockOn_->GetTarget(); // コピーしておく
		for (std::list<const IEnemy*>::iterator targetItr = targets.begin(); targetItr != targets.end(); ++targetItr) {
			// ロックオン対象がいるかつ生きてたら対象に向かって弾を撃つ
			if ((*targetItr) && !(*targetItr)->GetIsDead()) {
				// レティクルのworld座標にターゲットのworld座標を入れる
				Vector3 targetWorldPos = (*targetItr)->GetWorldPosition();
				Vector3 diff = targetWorldPos - WorldPos;
				diff = Normalize(diff);
				velocity = Normalize(velocity);
				velocity = Multiply(bulletSpeed_, diff);
				// プールから取ってくる
				IBullet* baseBullet = bulletObjectPool_->GetBullet("player");
				// 取ってこれたかチェックする
				if (baseBullet) {
					PlayerBullet* bullet = dynamic_cast<PlayerBullet*>(baseBullet);
					bullet->SetIsActive(true);
					bullet->SetLockOn(lockOn_);
					bullet->SetPosition(WorldPos);
					bullet->SetVelocity(velocity);
				}
			}
		}
	}
	else {
		Vector3 reticleWorldPos = lockOn_->GetWorldPosition3DReticle();
		velocity = reticleWorldPos - WorldPos;
		velocity = Normalize(velocity);
		velocity = bulletSpeed_ * velocity;

		// 弾を生成し、初期化
		// プールから取ってくる
		IBullet* baseBullet = bulletObjectPool_->GetBullet("player");
		// 取ってこれたかチェックする
		if (baseBullet) {
			PlayerBullet* bullet = dynamic_cast<PlayerBullet*>(baseBullet);
			bullet->SetIsActive(true);
			bullet->SetLockOn(lockOn_);
			const float kDistanceZ = 10.0f;
			Vector3 position = WorldPos;
			position.z = position.z + kDistanceZ;
			bullet->SetPosition(position);
			bullet->SetVelocity(velocity);
		}
	}
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
