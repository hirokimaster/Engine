#include "LockOn.h"
#include "application/GameObject/Player/Player.h"

void LockOn::Update(const std::list<std::unique_ptr<Enemy>>& enemies, const Camera& camera)
{
	XINPUT_STATE joyState{};

	// ロックオン状態なら
	if (target_) {

		if (Input::GetInstance()->GetJoystickState(joyState)) {

			if (Input::GetInstance()->PressedButton(XINPUT_GAMEPAD_LEFT_SHOULDER)) {
				// ロックオンを外す
				target_ = nullptr;
			}
		}
		else if (UnLock(camera)) {
			// ロックオンを外す
			target_ = nullptr;
		}
		
		
	}
	else {
		// 検索
		if (Input::GetInstance()->GetJoystickState(joyState)) {

			if (Input::GetInstance()->PressedButton(XINPUT_GAMEPAD_LEFT_SHOULDER)) {
				Search(enemies, camera);
			}
		}
	}

	// reticleの位置を決める
	ReticlePositionCalc(camera);

	ImGui::Begin("reticle");
	ImGui::Text("ret = %f.2, %f.2", &positionScreen_);
	ImGui::End();

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
	target_ = nullptr;

	if (!targets.empty()) {
		// ロックオン対象を設定 (最初の敵を選択)
		target_ = targets.front();
	}
}

bool LockOn::UnLock(const Camera& camera)
{
	Vector3 positionWorld = target_->GetWorldPosition();
	Vector3 positionScreen = TransformPositionScreen(positionWorld, camera);

	// 距離判定
	if (CheckReticleRange(positionScreen)) {
		return false;
	}

	return true;
}

void LockOn::ReticlePositionCalc(const Camera& camera)
{
	// ロックオン状態なら
	if (target_ && !target_->GetIsDead()) {
		// 敵のロックオン座標
		Vector3 positionWorld = target_->GetWorldPosition();
		Vector3 positionScreen = TransformPositionScreen(positionWorld, camera);

		// Vector2に入れる
		positionScreen_ = Vector2(positionScreen.x, positionScreen.y);
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

	float reticleRadius = 25.0f; // レティクルの半径

	Vector2 reticleCenter = player_->GetScreenPosition2DReticle();

	Vector2 enemyPosition = Vector2(position.x,position.y); // enemyのスクリーン座標

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
