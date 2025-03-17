/**
* @file LockOn.cpp
* @brief プレイヤーのロックオン機能、レティクル
* @author 仁平 琉乃
*/

#include "LockOn.h"

void LockOn::Initialize()
{
	TextureManager::Load("resources/Player/reticle.png");
	sprite2DReticle_.reset(Sprite::Create(TextureManager::GetTexHandle("Player/reticle.png"), reticlePosition_));
	spriteLockOnReticle_.reset(Sprite::Create(TextureManager::GetTexHandle("Player/reticle.png"), { 640.0f,360.0f }));
	spriteLockOnReticle_->SetAnchorPoint(Vector2(0.5f, 0.5f));
	sprite2DReticle_->SetAnchorPoint(Vector2(0.5f, 0.5f));
	worldTransform3DReticle_.Initialize();
	debugReticle_ = std::make_unique<Object3DPlacer>();
	debugReticle_->Initialize();
	debugReticle_->SetModel("Player/cube.obj");
	debugReticle_->SetTexHandle(TextureManager::GetTexHandle("TempTexture/white.png"));
}

void LockOn::Update(const std::list<std::unique_ptr<Enemy>>& enemies, const Camera& camera)
{
	XINPUT_STATE joyState{};

	// ロックオン状態なら
	if (!target_.empty()) {

		if (Input::GetInstance()->PressedButton(XINPUT_GAMEPAD_A)) {
			// ロックオンを外す
			target_.clear();
			isLockOnMode_ = false;
		}

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
			sprite.reset(Sprite::Create(TextureManager::GetTexHandle("Player/reticle.png"), { (*itr).x,(*itr).y }));
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

	debugReticle_->Update();
}

void LockOn::Draw(const Camera& camera)
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

	debugReticle_->Draw(camera);
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
	Reticle(camera);
	playerPosition;
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
