#include "LockOn.h"

void LockOn::Initialize()
{
	texHandle2DReticle_ = TextureManager::Load("resources/reticle.png");
	sprite2DReticle_.reset(Sprite::Create(texHandle2DReticle_, { 640.0f,360.0f }));
	sprite2DReticle_->SetAnchorPoint(Vector2(0.5f, 0.5f));
	worldTransform3DReticle_.Initialize();
}

void LockOn::Update(const std::list<std::unique_ptr<Enemy>>& enemies, const Camera& camera)
{
	XINPUT_STATE joyState{};

	// ロックオン状態なら
	if (!target_.empty()) {

		if (Input::GetInstance()->GetJoystickState(joyState)) {

			if (Input::GetInstance()->PressedButton(XINPUT_GAMEPAD_LEFT_SHOULDER)) {
				// ロックオンを外す
				target_.clear();
				isLockOnMode_ = false;
			}
		}
		else {
			// アンロック判定
			for (auto itr = target_.begin(); itr != target_.end(); ) {
				if (UnLock(camera, *itr)) {
					itr = target_.erase(itr);
				}
				else {
					++itr;
				}
			}
		}

	}
	else {
		// 空だったらモードを勝手に外す
		isLockOnMode_ = false;

		// 検索
		if (Input::GetInstance()->GetJoystickState(joyState)) {

			if (Input::GetInstance()->PressedButton(XINPUT_GAMEPAD_LEFT_SHOULDER)) {
				startLockOnTimer_ = true;
				isLockOnMode_ = true;
			}
		}
	}

	if (startLockOnTimer_) {
		--lockOnTimer_;
	}

	if (lockOnTimer_ <= 0.0f) {
		isLockOnMode_ = false;
	}

	// ロックオンモードになったとき検索をし始める
	if (isLockOnMode_) {
		Search(enemies, camera);
	}
	else {
		lockOnTimer_ = 60.0f;
	}

	for (auto itr = target_.begin(); itr != target_.end(); ++itr) {
		if ((*itr)->GetIsDead()) {
			target_.erase(itr);
		}
	}

	// reticleの位置を決める
	ReticlePositionCalc(camera);

	if (!target_.empty()) {

		for (auto itr = positionScreen_.begin(); itr != positionScreen_.end(); ++itr) {
			std::unique_ptr<Sprite> sprite;
			sprite.reset(Sprite::Create(texHandle2DReticle_, { (*itr).x,(*itr).y }));
			sprite->SetAnchorPoint({ 0.5f,0.5f });
			sprite_.push_back(std::move(sprite));
		}

	}

	ImGui::Begin("reticle");
	ImGui::Text("ret = %f, %f", &positionScreen_);
	ImGui::End();

}

void LockOn::Draw()
{
	sprite2DReticle_->Draw();

	for (auto itr = sprite_.begin(); itr != sprite_.end(); ++itr) {
		(*itr)->Draw();
	}
}

void LockOn::Reticle(const Camera& camera, const Vector2& position, const Vector3& playerPosition)
{
	// ビューポート行列
	Matrix4x4 matViewport =
		MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);

	// ビュープロジェクションビューポート合成行列
	Matrix4x4 matVPN =
		Multiply(camera.matView, Multiply(camera.matProjection, matViewport));
	// 逆行列を計算
	Matrix4x4 matInverseVPN = Inverse(matVPN);

	// スクリーン座標
	Vector3 posNear = Vector3((float)position.x, (float)position.y, 1);
	Vector3 posFar = Vector3((float)position.x, (float)position.y, 0);

	// スクリーン座標からワールド座標系へ
	posNear = Transform(posNear, matInverseVPN);
	posFar = Transform(posFar, matInverseVPN);

	// マウスレイの方向
	Vector3 mouseDirection = Subtract(posNear, posFar);
	mouseDirection = Normalize(mouseDirection);

	// カメラから照準オブジェクトの距離
	const float kDistanceTestObject = playerPosition.z + 100.0f;
	worldTransform3DReticle_.translate = Multiply(kDistanceTestObject, mouseDirection);
	worldTransform3DReticle_.UpdateMatrix();
}

Vector3 LockOn::GetWorldPosition3DReticle() const
{
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform3DReticle_.matWorld.m[3][0];
	worldPos.y = worldTransform3DReticle_.matWorld.m[3][1];
	worldPos.z = worldTransform3DReticle_.matWorld.m[3][2];

	return worldPos;
}

void LockOn::UpdateReticle(const Camera& camera, const Vector3& playerPosition)
{
	POINT mousePosition;
	// マウス座標(スクリーン座標)を取得する
	GetCursorPos(&mousePosition);

	// クライアントエリア座標に変換する
	HWND hwnd = WinApp::GetInstance()->GetHwnd();
	ScreenToClient(hwnd, &mousePosition);

	Vector2 spritePosition = sprite2DReticle_->GetPosition();

	XINPUT_STATE joyState{};

	// ジョイスティック状態取得
	if (Input::GetInstance()->GetJoystickState(joyState)) {
		spritePosition.x += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * 5.0f;
		spritePosition.y -= (float)joyState.Gamepad.sThumbRY / SHRT_MAX * 5.0f;
		// スプライトの座標変更を反映
		sprite2DReticle_->SetPosition(spritePosition);
		screenPositionReticle_ = spritePosition;
	}

	// レティクル
	Reticle(camera, Vector2((float)spritePosition.x, (float)spritePosition.y), playerPosition);
}

void LockOn::Search(const std::list<std::unique_ptr<Enemy>>& enemies, const Camera& camera)
{
	// ロックオン目標
	std::list<const Enemy*> targets;

	// すべての敵に対して順にロックオン判定
	for (const std::unique_ptr<Enemy>& enemy : enemies) {
		// screen座標
		Vector3 positionWorld = enemy->GetWorldPosition();
		Vector3 positionScreen = TransformPositionScreen(positionWorld, camera);

		if (CheckReticleRange(positionScreen)) {
			targets.push_back(enemy.get());
		}
	}

	// ロックオン対象をリセット
	target_.clear();

	if (!targets.empty()) {
		// ロックオン対象を設定
		for (const Enemy* target : targets) {
			target_.push_back(target);
		}
	}
}

bool LockOn::UnLock(const Camera& camera, const Enemy* target)
{
	Vector3 positionWorld = target->GetWorldPosition();
	Vector3 positionScreen = TransformPositionScreen(positionWorld, camera);

	// 距離判定
	if (CheckReticleRange(positionScreen)) {
		return false;
	}

	return true;
}

void LockOn::ReticlePositionCalc(const Camera& camera)
{
	positionScreen_.clear();
	sprite_.clear();

	for (const Enemy* target : target_) {
		// ロックオン状態なら
		if (target && !target->GetIsDead()) {
			// 敵のロックオン座標
			Vector3 positionWorld = target->GetWorldPosition();
			Vector3 positionScreen = TransformPositionScreen(positionWorld, camera);

			// Vector2に入れる
			positionScreen_.push_back(Vector2(positionScreen.x, positionScreen.y));
		}
	}

}

Vector3 LockOn::TransformPositionWorld(const Vector3& positionWorld, const Camera& camera)
{
	Vector3 positionView = Transform(positionWorld, camera.matView);

	return positionView;
}

bool LockOn::CheckReticleRange(const Vector3& position)
{
	// レティクルの範囲内にあるかを判定

	float reticleRadius = 1280.0f; // レティクルの半径

	Vector2 reticleCenter = screenPositionReticle_;

	Vector2 enemyPosition = Vector2(position.x, position.y); // enemyのスクリーン座標

	float distance = Length(enemyPosition - reticleCenter); // 距離

	if (distance <= reticleRadius) {
		return true;
	}

	return false;
}

Vector3 LockOn::TransformPositionScreen(const Vector3& positionWorld, const Camera& camera)
{
	// worldからscreenに変換
	Matrix4x4 matViewport = MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
	Matrix4x4 matViewProjectionViewport = camera.matView * camera.matProjection * matViewport;

	return Transform(positionWorld, matViewProjectionViewport);
}
