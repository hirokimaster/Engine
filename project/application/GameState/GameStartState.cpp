/**
* @file GameStartState.cpp
* @brief ゲームスタート演出
* @author 仁平 琉乃
*/


#include "GameStartState.h"

void GameStartState::Initialize()
{
	cameraManager_ = CameraManager::GetInstance();
	// 調整項目追加
	AddAdjustmentVariables();
	// 調整項目適用
	ApplyAdjustmentVariables();
	// カメラに初期値を入れとく
	cameraManager_->GetFollowCamera()->SetOffset(offsetStart_);
	cameraManager_->SetCameraRotate(cameraRotateStart_);
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

void GameStartState::AddAdjustmentVariables()
{
	AdjustmentVariables* variables = AdjustmentVariables::GetInstance();
	const char* groupName = "GameStartParam";
	// グループを追加
	variables->CreateGroup(groupName);
	variables->AddItem(groupName, "cameraOffsetStart", offsetStart_);
	variables->AddItem(groupName, "cameraOffsetEnd", offsetEnd_);
	variables->AddItem(groupName, "cameraRotateStart", cameraRotateStart_);
	variables->AddItem(groupName, "cameraRotateEnd", cameraRotateEnd_);
}

void GameStartState::ApplyAdjustmentVariables()
{
	AdjustmentVariables* variables = AdjustmentVariables::GetInstance();
	const char* groupName = "GameStartParam";
	offsetStart_ = variables->GetValue<Vector3>(groupName, "cameraOffsetStart");
	offsetEnd_ = variables->GetValue<Vector3>(groupName, "cameraOffsetEnd");
	cameraRotateStart_ = variables->GetValue<Vector3>(groupName, "cameraRotateStart");
	cameraRotateEnd_ = variables->GetValue<Vector3>(groupName, "cameraRotateEnd");
}
																		