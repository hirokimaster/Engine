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
	BaseIndividualObject::Initialize("Player/player.obj", "TempTexture/white.png", ColliderType::Sphere);
	object_.lock()->SetPosition({ 0,40.0f,-1500.0f });
	object_.lock()->SetColor({ 1.0f,1.0f,1.0f,0.0f });
	object_.lock()->SetCategory(DrawCategory::Foreground);
	// 属性設定
	collider_->SetCollosionAttribute(kCollisionAttributePlayer); // 自分の属性
	collider_->SetCollisionMask(kCollisionAttributeEnemyBullet); // 当たる対象
	collider_->SetRadious(3.0f);

	// 調整項目
	AddAdjustmentVariables();

	// 死亡フラグ
	isDead_ = false;
	deadTimer_ = 60.0f;

	// 発射タイマー
	attackTimer_ = 0.0f;
	gameStartTimer_ = 120.0f;

	// particle
	engineParticle_ = std::make_unique<EngineParticle>();
	engineParticle_->Initialize();

	// UI
	spriteAttack_.reset(Sprite::Create(TextureManager::GetTexHandle("UI/RB.png"), { 1000.0f , 500.0f }));
	spriteAttack_->SetScale({ 2.0f,2.0f });
	spriteMove_.reset(Sprite::Create(TextureManager::GetTexHandle("UI/L.png"), { 240.0f,500.0f }));
	spriteMove_->SetScale({ 2.0f,2.0f });

	// 影
	shadow_ = std::make_unique<PlaneProjectionShadow<WorldTransform>>();
	shadow_->Initialize("Player/player.obj", &object_.lock()->GetWorldTransform());
	shadow_->SetScale({ 2.0f,1.0f,2.0f });
	shadow_->SetOffset({ 0.0f,-8.0f,80.0f });
	shadow_->SetIsActive(false);
}

void Player::Update()
{
	--gameStartTimer_;
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

	BaseIndividualObject::Update(); // object共通の更新処理
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

	// 影
	shadow_->Update();

	// ゲーム開始演出中または死亡時は本体,影を非表示
	if (isDead_ || gameStartTimer_ > 40.0f) {
		object_.lock()->SetColor({ 1.0f,1.0f,1.0f,0.0f });
		engineParticle_->SetIsActive(false);
	} else {
		object_.lock()->SetColor({ 1.0f,1.0f,1.0f,1.0f });
		shadow_->SetIsActive(true);
		object_.lock()->SetCategory(DrawCategory::Background);
		// particle
		if (auto obj = object_.lock()) {
			engineParticle_->Update(obj->GetWorldTransform().rotate, obj->GetWorldTransform().translate);
		}
	}

	Move();
	ApplyAdjustmentVariables();
}

void Player::Move()
{
    XINPUT_STATE joyState{};
    Vector3 spriteMove{};

    if (!isDead_) {


        if (gameStartTimer_ <= 0.0f) {
  
            // ジョイスティック状態取得
            if (Input::GetInstance()->GetJoystickState(joyState)) {
                spriteMove.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * 2.0f;
                spriteMove.y += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * 2.0f;
            }

			gameStartTimer_ = 0.0f;
        }

		Vector3 reticlePos = lockOn_->GetWorldPosition3DReticle();
		Vector3 playerPos = GetWorldPosition(); // プレイヤーの現在位置
		// 追従速度
		const float followSpeed = 0.1f;
		// 補間
		Vector3 newPos = playerPos + followSpeed * (reticlePos - playerPos);
		object_.lock()->SetPosition(newPos);

        spriteMove_->SetPosition(Vector2((5.0f * spriteMove.x) + 240.0f, (5.0f * spriteMove.y) + 500.0f));
    }
}

void Player::Attack()
{
	if (!isDead_) {

		// ターゲットがいたらロックオン
		if (!lockOn_->GetTarget().empty()) {
			if (lockOn_->GetIsLockOnMode()) {
				MultiLockOnFire(GetWorldPosition());
			}
			else {
				LockOnFire(GetWorldPosition());
			}
		}
		// それ以外はノーマル
		else {
			NormalFire(GetWorldPosition());
		}
	}
	
}

void Player::FireBullet(const Vector3& position, const Vector3& velocity)
{
	// poolから取り出す
	IBullet* baseBullet = bulletObjectPool_->GetBullet("player");
	// 取り出せてたら値を設定する
	if (baseBullet) {
		Vector3 dir = { 0.0f,0.0f,5.0f };
		Vector3 newPosition = position + dir;
		PlayerBullet* bullet = dynamic_cast<PlayerBullet*>(baseBullet);
		bullet->SetIsActive(true); // アクティブにする
		bullet->SetLockOn(lockOn_);
		bullet->SetPosition(newPosition); // 位置
		bullet->SetVelocity(velocity); // 速度
	}
}

void Player::LockOnFire(const Vector3& position)
{
	const IEnemy* nearestEnemy = nullptr;
	float minDistance = (std::numeric_limits<float>::max)();
	std::list<const IEnemy*> targets = lockOn_->GetTarget();

	// 一番近い敵探す
	for (const IEnemy* enemy : targets) {
		if (enemy && !enemy->GetIsDead()) {
			Vector3 toEnemy = enemy->GetWorldPosition() - position;
			float dist = Length(toEnemy);

			if (dist < minDistance) {
				minDistance = dist;
				nearestEnemy = enemy;
			}
		}
	}

	// 一番近いやつにだけ撃つ
	if (nearestEnemy) {
		Vector3 targetWorldPos = nearestEnemy->GetWorldPosition();
		Vector3 diff = Normalize(targetWorldPos - position);
		Vector3 velocity = bulletSpeed_ * diff;
		FireBullet(position, velocity); // 弾を発射
	}
}

void Player::NormalFire(const Vector3& position)
{
	Vector3 reticleWorldPos = lockOn_->GetWorldPosition3DReticle();
	Vector3 velocity = reticleWorldPos - position;
	velocity = bulletSpeed_ * Normalize(velocity);

	// 弾を発射
	FireBullet(position, velocity); // 弾を発射
}

void Player::MultiLockOnFire(const Vector3& position)
{
	// 弾の速度
	Vector3 velocity = { 0,0,bulletSpeed_ };
	std::list<Vector3> lockOnVelocity;
	// 自機から照準オブジェクトのベクトル
	Vector3 WorldPos = GetWorldPosition();

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
				IBullet* baseBullet = bulletObjectPool_->GetBullet("player");
				if (baseBullet) {
					Vector3 dir = { 0.0f,0.0f,5.0f };
					Vector3 newPosition = position + dir;
					PlayerBullet* bullet = dynamic_cast<PlayerBullet*>(baseBullet);
					bullet->SetIsActive(true); // アクティブにする
					bullet->SetLockOn(lockOn_);
					bullet->SetPosition(newPosition); // 位置
					bullet->SetVelocity(velocity); // 速度
				}
			}
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
	// 範囲
	const float kRotateLimitZ = 0.7f;
	const float kRotateLimitX = 0.15f;
	const float kLerpFactor = 0.5f;
	
	// ゲームパッドの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;

	if (!isDead_) {
		if (gameStartTimer_ <= 0.0f) {
			// ゲームパッド状態取得
			if (Input::GetInstance()->GetJoystickState(joyState)) {
				// ゲームパッドの入力から回転速度を計算
				rotate.z += (float)joyState.Gamepad.sThumbLX / SHRT_MAX;
				rotate.x += (float)joyState.Gamepad.sThumbLY / SHRT_MAX;
			}
			gameStartTimer_ = 0.0f;
		}
		
	}
	
	Vector3 rotateVelo{};
	// 回転を適用
	rotateVelo.z = std::lerp(object_.lock()->GetWorldTransform().rotate.z, object_.lock()->GetWorldTransform().rotate.z - rotate.z, kLerpFactor);
	rotateVelo.x = std::lerp(object_.lock()->GetWorldTransform().rotate.x, object_.lock()->GetWorldTransform().rotate.x - rotate.x, kLerpFactor);

	// z軸に制限をかける
	rotateVelo.z = std::clamp(rotateVelo.z, -kRotateLimitZ, kRotateLimitZ);

	// x軸に制限をかける
	rotateVelo.x = std::clamp(rotateVelo.x, -kRotateLimitX, kRotateLimitX);

	// y軸は回転させないので0にしとく
	rotateVelo.y = 0.0f;

	BaseIndividualObject::SetRotate(rotateVelo);
}

void Player::IncurDamage()
{
	// 敵の攻撃を食らったら
	const uint32_t kDamage = 3; // 敵からの攻撃ダメージ
	if (isHitEnemyFire_) {
		hp_ -= kDamage;
		isHitEnemyFire_ = false;
	}

	// hpが0になったら死ぬ
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
	ImGui::Text("position = %f : %f : %f", object_.lock()->GetWorldTransform().translate.x,
		object_.lock()->GetWorldTransform().translate.y, object_.lock()->GetWorldTransform().translate.z);
	ImGui::End();

	ImGui::Begin("PlayerRotate");
	ImGui::Text("rotate [x: %.3f ] [y: %.3f] [z: %.3f]", object_.lock()->GetWorldTransform().rotate.x,
		object_.lock()->GetWorldTransform().rotate.y, object_.lock()->GetWorldTransform().rotate.z);
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
	worldPos.x = object_.lock()->GetWorldTransform().matWorld.m[3][0];
	worldPos.y = object_.lock()->GetWorldTransform().matWorld.m[3][1];
	worldPos.z = object_.lock()->GetWorldTransform().matWorld.m[3][2];

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
	// jsonから読み込んだ値を適用
	AdjustmentVariables* variables = AdjustmentVariables::GetInstance();
	const char* groupName = "Player";
	moveSpeed_ = variables->GetValue<float>(groupName, "moveSpeed");
	rotate_ = variables->GetValue<Vector3>(groupName, "rotate");
	object_.lock()->SetRotate(rotate_);
	bulletSpeed_ = variables->GetValue<float>(groupName, "bulletSpeed");
	hp_ = variables->GetValue<int32_t>(groupName, "hp");
	moveMinLimit_ = variables->GetValue<Vector3>(groupName, "moveMinLimit");
	moveMaxLimit_ = variables->GetValue<Vector3>(groupName, "moveMaxLimit");
	rotateSpeed_ = variables->GetValue<float>(groupName, "rotateSpeed");
	rotateLerpFactor_ = variables->GetValue<float>(groupName, "rotateLerpFactor");
}

