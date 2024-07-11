#include "LockOn.h"

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

}

void LockOn::Search(const std::list<std::unique_ptr<Enemy>>& enemies, const Camera& camera)
{
	// ロックオン目標
	std::list<std::pair<float, const Enemy*>> targets;

	// すべての敵に対して順にロックオン判定
	for (const std::unique_ptr<Enemy>& enemy : enemies) {
		// view座標
		Vector3 positionView = TransformPositionWorld(enemy->GetWorldPosition(), camera);
		
		if (CheckDistance(positionView)) {
			targets.emplace_back(std::make_pair(positionView.z, enemy.get()));
		}
	}

	// ロックオン対象をリセット
	target_ = nullptr;

	if (!targets.empty()) {
		// 距離で昇順にソート
		targets.sort([](auto& pair1, auto& pair2) {return pair1.first < pair2.first; });
		// ソートの結果一番近い敵をロックオン対象にする
		target_ = targets.front().second;
	}
}

bool LockOn::UnLock(const Camera& camera)
{
	// view座標
	Vector3 positionView = TransformPositionWorld(target_->GetWorldPosition(), camera);

	// 距離判定
	if (CheckDistance(positionView)) {
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
		// worldからscreenに変換
		// ビューポート行列
		Matrix4x4 matViewport =
			MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);

		// ビュー行列とプロジェクション行列、ビューポート行列を合成する
		Matrix4x4 matViewProjectionViewport =
			camera.matView * camera.matProjection * matViewport;

		Vector3 positionScreen = Transform(positionWorld, matViewProjectionViewport);
		// Vector2に入れる
		positionScreenV2_ = Vector2(positionScreen.x, positionScreen.y);
		
	}
}

bool LockOn::CheckDistance(const Vector3& positionView)
{
	if (minDistance_ <= positionView.z && positionView.z <= maxDistance_) {
		return true;
	}

	return false;
}

Vector3 LockOn::TransformPositionWorld(const Vector3& positionWorld, const Camera& camera)
{
	Vector3 positionView = Transform(positionWorld, camera.matView);

	return positionView;
}
