/**
* @file LockOn.cpp
* @brief プレイヤーのロックオン機能、レティクル
* @author 仁平 琉乃
*/

#include "LockOn.h"

void LockOn::Initialize()
{
	texHandle2DReticle_ = TextureManager::Load("resources/Player/reticle2.png");
	sprite2DReticle_.reset(Sprite::Create(texHandle2DReticle_, reticlePosition_));
	spriteLockOnReticle_.reset(Sprite::Create(texHandle2DReticle_, { 640.0f,360.0f }));
	spriteLockOnReticle_->SetAnchorPoint(Vector2(0.5f, 0.5f));
	sprite2DReticle_->SetAnchorPoint(Vector2(0.5f, 0.5f));
	worldTransform3DReticle_.Initialize();
}

void LockOn::Update(const std::list<std::unique_ptr<Enemy>>& enemies, const Camera& camera)
{
	XINPUT_STATE joyState{};

	// ロックオン状態なら
	if (!target_.empty()) {

		if (Input::GetInstance()->GetJoystickState(joyState)) {

			if (Input::GetInstance()->PressedButton(XINPUT_GAMEPAD_A)) {
				// ロックオンを外す
				target_.clear();
				isLockOnMode_ = false;
			}
		}
		else {
			// アンロック判定
			for (auto itr = target_.begin(); itr != target_.end();) {
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
		
	}

	// 検索
	Search(enemies, camera);

	// ターゲットが死んだら消す
	for (auto itr = target_.begin(); itr != target_.end(); ++itr) {
		if ((*itr)->GetIsDead()) {
			target_.erase(itr);
		}
	}

	// reticleの位置を決める
	ReticlePositionCalc(camera);

	// ロックオンした敵にレティクルを出す
	if (!target_.empty()) {

		for (auto itr = positionScreen_.begin(); itr != positionScreen_.end(); ++itr) {
			std::unique_ptr<Sprite> sprite;
			sprite.reset(Sprite::Create(texHandle2DReticle_, { (*itr).x,(*itr).y }));
			sprite->SetAnchorPoint({ 0.5f,0.5f });
			sprite_.push_back(std::move(sprite));
		}

	}
	else {
		positionScreen_.clear();
		sprite_.clear();
	}

	// 範囲制限
	ReticleRangeLimit();
}

void LockOn::Draw()
{

	if (isLockOnMode_) {
		spriteLockOnReticle_->Draw();
	}
	else {
		sprite2DReticle_->Draw();
	}

	for (const auto& sprite : sprite_) {
		sprite->Draw();
	}
}

void LockOn::Reticle(const Camera& camera, const Vector2& position, const Vector3& playerPosition)
{

	sprite2DReticle_->SetPosition(position);

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
	const float kDistanceTestObject = 100.0f;
	worldTransform3DReticle_.translate = kDistanceTestObject * mouseDirection + playerPosition;
	worldTransform3DReticle_.UpdateMatrix();
}

void LockOn::ReticleRangeLimit()
{
	// reticleの範囲制限
	Vector2 positionLockOn = spriteLockOnReticle_->GetPosition();
	Vector2 positionReticle = sprite2DReticle_->GetPosition();
	positionReticle.x = std::clamp(positionReticle.x, 25.0f, 1255.0f);
	positionReticle.y = std::clamp(positionReticle.y, 25.0f, 695.0f);

	positionLockOn.x = std::clamp(positionLockOn.x, 25.0f, 1255.0f);
	positionLockOn.y = std::clamp(positionLockOn.y, 25.0f, 695.0f);

	sprite2DReticle_->SetPosition(positionReticle);
	spriteLockOnReticle_->SetPosition(positionLockOn);
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

void LockOn::UpdateReticle(const Camera& camera, const Vector3& playerPosition, bool isGameStart)
{
	POINT mousePosition;
	// マウス座標(スクリーン座標)を取得する
	GetCursorPos(&mousePosition);

	// クライアントエリア座標に変換する
	HWND hwnd = WinApp::GetInstance()->GetHwnd();
	ScreenToClient(hwnd, &mousePosition);

	Vector2 spritePosition;

	if (isLockOnMode_) {
		spritePosition = spriteLockOnReticle_->GetPosition();
	}
	else {
		spritePosition = sprite2DReticle_->GetPosition();
		spriteLockOnReticle_->SetPosition(spritePosition);
	}

	if (isGameStart) {
		XINPUT_STATE joyState{};

		// ジョイスティック状態取得
		if (Input::GetInstance()->GetJoystickState(joyState)) {
			spritePosition.x += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * 12.0f;
			spritePosition.y -= (float)joyState.Gamepad.sThumbRY / SHRT_MAX * 12.0f;
			// スプライトの座標変更を反映
			if (isLockOnMode_) {
				spriteLockOnReticle_->SetPosition(spritePosition);
				sprite2DReticle_->SetPosition(spritePosition);
			}
			else {
				sprite2DReticle_->SetPosition(spritePosition);
			}

			screenPositionReticle_ = spritePosition;
		}
	}
	
	// レティクル
	Reticle(camera, Vector2((float)spritePosition.x, (float)spritePosition.y), playerPosition);
}

void LockOn::Search(const std::list<std::unique_ptr<Enemy>>& enemies, const Camera& camera)
{
	// ロックオン目標
	std::list<const Enemy*> targets;

	for (const std::unique_ptr<Enemy>& enemy : enemies) {
		if (enemy->GetIsSortie()) {
			if (isLockOnMode_) {

				// screen座標
				Vector3 positionWorld = enemy->GetWorldPosition();
				Vector3 positionScreen = TransformPositionScreen(positionWorld, camera);

				// すでにロックオンされているかチェック
				bool alreadyLockedOn = std::find(target_.begin(), target_.end(), enemy.get()) != target_.end();

				// 新たにロックオンするか、すでにロックオンされている場合は対象として残す
				if (alreadyLockedOn || CheckReticleRange(positionScreen)) {
					targets.push_back(enemy.get());
				}

			}
			else {


				// screen座標
				Vector3 positionWorld = enemy->GetWorldPosition();
				Vector3 positionScreen = TransformPositionScreen(positionWorld, camera);

				if (CheckReticleRange(positionScreen)) {
					targets.push_back(enemy.get());
				}

			}
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

	float reticleRadius; // レティクルの範囲

	if (isLockOnMode_) {
		reticleRadius = 30.0f;
	}
	else {
		reticleRadius = 30.0f;
	}

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
