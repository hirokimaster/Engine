#include "GameStartState.h"

void GameStartState::Initialize()
{
	cameraManager_ = CameraManager::GetInstance();
	// 初期のカメラのオフセットと回転
	offsetStart_ = { -12.0f, 0.5f, 13.0f };
	cameraRotateStart_ = { 0, std::numbers::pi_v<float> *3.0f / 4.0f, 0 };
	// 目的のカメラのオフセットと回転
	offsetEnd_ = { 0, 1.5f, -25.0f };
	cameraRotateEnd_ = { 0, 0, 0 };
	rotateParam_ = 0.0f;
	// 初期値を入れとく
	cameraManager_->GetFollowCamera()->SetOffset(offsetEnd_);
	cameraManager_->SetCameraRotate(cameraRotateEnd_);
}

void GameStartState::Update()
{
	// 補間が終わってない時
	if (rotateParam_ <= 1.0f) {
		// カメラの位置を補間する
		Vector3 currentOffset = Lerp(offsetStart_, offsetEnd_, rotateParam_);
		// 回転の補間をする
		Vector3 currentCameraRotate = Lerp(cameraRotateStart_, cameraRotateEnd_, rotateParam_);
		// 正規化
		currentCameraRotate.y = NormalizeRotation(currentCameraRotate.y);
		// カメラに適用
		cameraManager_->GetFollowCamera()->SetOffset(currentOffset);
		cameraManager_->SetCameraRotate(currentCameraRotate);
		rotateParam_ += 0.02f;
	}

	// 補間が終わったら
	if (rotateParam_ > 1.0f) {
		rotateParam_ = 1.0f;
	}
}
