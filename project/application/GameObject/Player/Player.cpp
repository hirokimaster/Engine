#include "Player.h"

void Player::Initialize(Object3DPlacer* object, uint32_t texHandle, const std::string& model)
{
	BaseObject::Initialize(object, texHandle, model); // 共通処理初期化
	texHandleBullet_ = TextureManager::Load("resources/uvChecker.png"); // bulletの画像
	object->SetColor(Vector4(1.0f, 0.0f, 1.0f, 1.0f));

	// reticle
	texHandle2DReticle_ = TextureManager::Load("resources/reticle.png");
	sprite2DReticle_.reset(Sprite::Create(texHandle2DReticle_, { 640.0f,360.0f }));
	sprite2DReticle_->SetAnchorPoint(Vector2(0.5f, 0.5f));
	worldTransform3DReticle_.Initialize();

	SetCollosionAttribute(kCollisionAttributePlayer);
	SetCollisionMask(kCollisionAttributeEnemy); // 当たる対象
}

void Player::Update()
{
	Move(); // 移動
	Attack(); // 攻撃
	UpdateBullet(); // 弾の更新
	BaseObject::Update(); // 共通の更新処理
}

void Player::UpdateReticle(const Vector3& position,const Camera& camera)
{
	Reticle(position);

	// 3Dレティクルの座標から2Dレティクルのスクリーン座標を計算
	Vector3 positionReticle = GetWorldPosition3DReticle();

	// ビューポート行列
	Matrix4x4 matViewport =
		MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);

	// ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4x4 matViewProjectionViewport =
		camera.matView * camera.matProjection * matViewport;

	// ワールド→スクリーン座標変換(ここで3Dから2Dになる)
	positionReticle = Transform(positionReticle, matViewProjectionViewport);

	// スプライトのレティクルに座標設定
	sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));
}

void Player::Draw(Camera& camera)
{
	BaseObject::Draw(camera); // 共通の描画処理

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
		std::unique_ptr<Object3DPlacer> objectBullet = std::make_unique<Object3DPlacer>();
		objectBullets_.push_back(std::move(objectBullet));
		bullet->Initialize(objectBullets_.back().get(), texHandleBullet_, "cube.obj");
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
			// 自機から照準オブジェクトのベクトル
			Vector3 WorldPos = GetWorldPosition();
			Vector3 ReticleWorldPos = GetWorldPosition3DReticle();
			velocity = ReticleWorldPos - WorldPos;
			velocity = Normalize(velocity);
			velocity = kBulletSpeed * velocity;
			// プレイヤーの向きに速度を合わせる
			//velocity = TransformNormal(velocity, worldTransform_.matWorld);
			// 弾を生成し、初期化
			std::unique_ptr<PlayerBullet> bullet = std::make_unique<PlayerBullet>();
			std::unique_ptr<Object3DPlacer> objectBullet = std::make_unique<Object3DPlacer>();
			objectBullets_.push_back(std::move(objectBullet));
			bullet->Initialize(objectBullets_.back().get(), texHandleBullet_, "cube.obj");
			bullet->SetPosition(WorldPos);
			bullet->SetVelocity(velocity);
			bullets_.push_back(std::move(bullet));
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

	std::list<std::unique_ptr<Object3DPlacer>>::iterator objectBulletsItr = objectBullets_.begin();	// objectのイテレータ

	// デスフラグが立ったら要素を削除
	bullets_.remove_if([&objectBulletsItr, this](std::unique_ptr<PlayerBullet>& bullet) {
		if (bullet->GetIsDead()) {
			// 対応するbulletObjectを削除
			objectBulletsItr = objectBullets_.erase(objectBulletsItr);
			return true;
		}
		else {
			++objectBulletsItr;
			return false;
		}
		});

}

void Player::OnCollision()
{

}

//void Player::Reticle(const Camera& camera, const Vector2& position)
//{
//	// スプライトのレティクルに座標設定
//	sprite2DReticle_->SetPosition(position);
//
//	// ビューポート行列
//	Matrix4x4 matViewport =
//		MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
//
//	// ビュープロジェクションビューポート合成行列
//	Matrix4x4 matVPN =
//		Multiply(camera.matView, Multiply(camera.matProjection, matViewport));
//	// 逆行列を計算
//	Matrix4x4 matInverseVPN = Inverse(matVPN);
//
//	// スクリーン座標
//	Vector3 posNear = Vector3((float)position.x, (float)position.y, 1);
//	Vector3 posFar = Vector3((float)position.x, (float)position.y, 0);
//
//	// スクリーン座標からワールド座標系へ
//	posNear = Transform(posNear, matInverseVPN);
//	posFar = Transform(posFar, matInverseVPN);
//
//	// マウスレイの方向
//	Vector3 mouseDirection = Subtract(posNear, posFar);
//	mouseDirection = Normalize(mouseDirection);
//	// カメラから照準オブジェクトの距離
//	const float kDistanceTestObject = 100.0f;
//	worldTransform3DReticle_.translate = Multiply(kDistanceTestObject, mouseDirection);
//	worldTransform3DReticle_.UpdateMatrix();
//}

void Player::Reticle(const Vector3& position)
{
	// 自機のワールド座標から3Dレティクルのワールド座標を計算
	// 自機から3Dレティクルへの距離
	const float kDistancePlayerTo3DReticle = position.z + 50.0f;
	// 自機から3Dレティクルへのオフセット(Z+向き)
	Vector3 offset = { 0, 0, 1.0f };
	// 自機のワールド行列の回転を反映
	offset = TransformNormal(offset, worldTransform_.matWorld);
	// ベクトルの長さを整える
	offset = Normalize(offset);
	offset = Multiply(kDistancePlayerTo3DReticle, offset);
	// 3Dレティクルの座標を設定
	worldTransform3DReticle_.translate.x = worldTransform_.translate.x + offset.x;
	worldTransform3DReticle_.translate.y = worldTransform_.translate.y + offset.y;
	worldTransform3DReticle_.translate.z = worldTransform_.translate.z + offset.z;
	worldTransform3DReticle_.UpdateMatrix();
}

void Player::DrawUI()
{
	sprite2DReticle_->Draw();
}

//void Player::UpdateReticle(const Camera& camera)
//{
//	POINT mousePosition;
//	// マウス座標(スクリーン座標)を取得する
//	GetCursorPos(&mousePosition);
//
//	// クライアントエリア座標に変換する
//	HWND hwnd = WinApp::GetInstance()->GetHwnd();
//	ScreenToClient(hwnd, &mousePosition);
//
//	Vector2 spritePosition = sprite2DReticle_->GetPosition();
//
//	XINPUT_STATE joyState{};
//
//	// ジョイスティック状態取得
//	if (Input::GetInstance()->GetJoystickState(joyState)) {
//		spritePosition.x += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * 5.0f;
//		spritePosition.y -= (float)joyState.Gamepad.sThumbRY / SHRT_MAX * 5.0f;
//		// スプライトの座標変更を反映
//		sprite2DReticle_->SetPosition(spritePosition);
//
//	}
//
//	// レティクル
//	Reticle(camera, Vector2((float)spritePosition.x, (float)spritePosition.y));
//}

Vector3 Player::GetWorldPosition()
{
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.matWorld.m[3][0];
	worldPos.y = worldTransform_.matWorld.m[3][1];
	worldPos.z = worldTransform_.matWorld.m[3][2];

	return worldPos;
}

Vector3 Player::GetWorldPosition3DReticle()
{
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform3DReticle_.matWorld.m[3][0];
	worldPos.y = worldTransform3DReticle_.matWorld.m[3][1];
	worldPos.z = worldTransform3DReticle_.matWorld.m[3][2];

	return worldPos;
}
