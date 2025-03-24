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
}

void CameraManager::Update()
{
	followCamera_->Update(camera_);
	camera_.UpdateMatrix();
}


