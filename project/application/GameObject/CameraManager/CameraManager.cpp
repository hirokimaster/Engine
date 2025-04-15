#include "CameraManager.h"

CameraManager* CameraManager::GetInstance()
{
	static CameraManager instance;
	return &instance;
}

void CameraManager::Initialize()
{
	camera_.Initialize(); // ノーマルカメラ
	followCamera_ = std::make_unique<FollowCamera>(); // 追従カメラ
	followCamera_->Initialize();
	Vector3 offset = { 0, 1.5f, -25.0f };
	followCamera_->SetOffset(offset);
}

void CameraManager::Update()
{
	followCamera_->Update(camera_);
	camera_.TransferMatrix();
}


