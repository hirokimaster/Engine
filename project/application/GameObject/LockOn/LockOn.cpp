/**
* @file LockOn.cpp
* @brief プレイヤーのロックオン機能、レティクル
* @author 仁平 琉乃
*/

#include "LockOn.h"

void LockOn::Initialize()
{
	TextureManager::Load("resources/Player/reticle.png");
	TextureManager::Load("resources/Player/lockOnReticle.png");
	sprite2DReticle_.reset(Sprite::Create(TextureManager::GetTexHandle("Player/reticle.png"), reticlePosition_));
	spriteLockOnReticle_.reset(Sprite::Create(TextureManager::GetTexHandle("Player/lockOnReticle.png"), { 640.0f,360.0f }));
	spriteLockOnReticle_->SetAnchorPoint(Vector2(0.5f, 0.5f));
	sprite2DReticle_->SetAnchorPoint(Vector2(0.5f, 0.5f));
	worldTransform3DReticle_.Initialize();

}

void LockOn::Update(const std::list<std::unique_ptr<IEnemy>>& enemies, const Camera& camera)
{
	XINPUT_STATE joyState{};

	// ロックオン状態なら
	if (!target_.empty()) {

		if (Input::GetInstance()->PressedButton(XINPUT_GAMEPAD_A)) {
			// ロックオンを外す
			target_.clear();
			isLockOnMode_ = false;
		}
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

		// プレイヤーの位置から最も近い敵を探す
		float minDistance = (std::numeric_limits<float>::max)();
		Vector3 nearestTarget;

		for (const auto& target : target_) {
			Vector3 diff = target->GetWorldPosition() - playerPosition_;
			float distance = Length(diff);
			if (distance < minDistance) {
				minDistance = distance;
				nearestTarget = target->GetWorldPosition();
			}
		}

		Vector3 screenPos = TransformPositionScreen(nearestTarget, camera);
		positionScreen_.push_back({ screenPos.x,screenPos.y });

		std::unique_ptr<Sprite> sprite;
		sprite.reset(Sprite::Create(TextureManager::GetTexHandle("Player/lockOnReticle.png"), { screenPos.x,screenPos.y }));
		sprite->SetAnchorPoint({ 0.5f,0.5f });
		sprite_.push_back(std::move(sprite));

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

	static float scale = 1.0f;
	static float scaleSpeed = 0.01f;
	static bool scalingUp = true;

	// スケール更新
	if (scalingUp) {
		scale += scaleSpeed;
		if (scale >= 1.3f) {
			scale = 1.3f;
			scalingUp = false;
		}
	}
	else {
		scale -= scaleSpeed;
		if (scale <= 1.0f) {
			scale = 1.0f;
			scalingUp = true;
		}
	}

	for (const auto& sprite : sprite_) {
		sprite->SetScale({ scale, scale });
		sprite->Draw();
	}
}

void LockOn::Reticle(const Camera& camera)
{
	Vector3 positionReticle = GetWorldPosition3DReticle();

	positionReticle = TransformPositionScreen(positionReticle, camera);

	sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));
}

void LockOn::ReticleRangeLimit()
{
	// reticleの範囲制限
	Vector2 positionLockOn = spriteLockOnReticle_->GetPosition();
	Vector2 positionReticle = sprite2DReticle_->GetPosition();
	positionReticle.x = std::clamp(positionReticle.x, 590.0f, 710.0f);
	positionReticle.y = std::clamp(positionReticle.y, 300.0f, 400.0f);

	positionLockOn.x = std::clamp(positionLockOn.x, 25.0f, 1255.0f);
	positionLockOn.y = std::clamp(positionLockOn.y, 25.0f, 695.0f);
	screenPositionReticle_ = positionReticle;

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
	Vector2 spritePosition;


	if (isLockOnMode_ || isGameStart) {
		spritePosition = spriteLockOnReticle_->GetPosition();
	}
	else {
		spritePosition = sprite2DReticle_->GetPosition();
		spriteLockOnReticle_->SetPosition(spritePosition);
	}

	const float kDistanceObject = 100.0f;
	Vector3 offset = { 0,0,1.0f };
	offset = kDistanceObject * Normalize(offset);
	worldTransform3DReticle_.translate = worldTransform3DReticle_.translate + offset;
	// playerとの距離
	worldTransform3DReticle_.UpdateMatrix();
	// レティクル
	playerPosition_ = playerPosition;
	Reticle(camera);

}

void LockOn::Search(const std::list<std::unique_ptr<IEnemy>>& enemies, const Camera& camera)
{
	// ロックオン目標
	std::list<const IEnemy*> targets;

	for (const std::unique_ptr<IEnemy>& enemy : enemies) {

		if (isLockOnMode_) {

			// screen座標
			Vector3 positionWorld = enemy->GetWorldPosition();
			Vector3 positionScreen = TransformPositionScreen(positionWorld, camera);

			// すでにロックオンされているかチェック
			bool alreadyLockedOn = std::find(target_.begin(), target_.end(), enemy.get()) != target_.end();

			// 新たにロックオンするか、すでにロックオンされている場合は対象として残す
			if (alreadyLockedOn || CheckReticleRange(positionScreen, positionWorld)) {
				targets.push_back(enemy.get());
			}

		}
		else {

			// screen座標
			Vector3 positionWorld = enemy->GetWorldPosition();
			Vector3 positionScreen = TransformPositionScreen(positionWorld, camera);

			if (CheckReticleRange(positionScreen, positionWorld)) {
				targets.push_back(enemy.get());
			}

		}

	}

	// ロックオン対象をリセット
	target_.clear();

	if (!targets.empty()) {
		// ロックオン対象を設定
		for (const IEnemy* target : targets) {
			target_.push_back(target);
		}
	}
}

bool LockOn::UnLock(const Camera& camera, const IEnemy* target)
{
	Vector3 positionWorld = target->GetWorldPosition();
	Vector3 positionScreen = TransformPositionScreen(positionWorld, camera);

	// 距離判定
	if (CheckReticleRange(positionScreen, positionWorld)) {
		return false;
	}

	return true;
}

void LockOn::ReticlePositionCalc(const Camera& camera)
{

	positionScreen_.clear();
	sprite_.clear();

	for (const IEnemy* target : target_) {
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

bool LockOn::CheckReticleRange(const Vector3& screenPosition, const Vector3& worldposition)
{
	// レティクルの範囲内にあるかを判定
	float reticleRadius; // レティクルの範囲

	if (isLockOnMode_) {
		reticleRadius = 20.0f;
	}
	else {
		reticleRadius = 20.0f;
	}

	Vector2 reticleCenter = screenPositionReticle_;
	Vector2 enemyPosition = Vector2(screenPosition.x, screenPosition.y); // enemyのスクリーン座標
	float screenDistance = Length(enemyPosition - reticleCenter); // スクリーン上での距離

	// ワールド座標での距離計算（3000以内）
	Vector3 enemyWorldPosition = worldposition;// 敵のワールド座標

	// プレイヤーと敵の距離を計算（3D空間で）
	float worldDistance = Length(enemyWorldPosition - playerPosition_);

	// 3000以内かつレティクル範囲内か
	if (screenDistance <= reticleRadius && worldDistance <= 4000.0f) {
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
