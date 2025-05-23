/**
* @file CameraManager.cpp
* @brief ゲーム中のカメラのマネージャ
* @author 仁平 琉乃
*/

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
	Vector3 offset = { 0, 1.5f, -20.0f };
	followCamera_->SetOffset(offset);

	shakeIntensity_ = 0.0f;
	shakeDuration_ = 0.0f;
	shakeTimeElapsed_ = 0.0f;
}

void CameraManager::Update()
{
	followCamera_->Update(camera_);

	Shake(); // シェイク

	camera_.UpdateMatrix();
}

void CameraManager::StartShake(float intensity, float duration)
{
	shakeIntensity_ = intensity;
	shakeDuration_ = duration;
	shakeTimeElapsed_ = 0.0f;
}

void CameraManager::ApplyShake()
{
	Vector3 shakeOffset(
		Random::Range(-8.0f, 8.0f) * shakeIntensity_,
		Random::Range(-8.0f, 8.0f) * shakeIntensity_,
		Random::Range(-8.0f, 8.0f) * shakeIntensity_
	);
	camera_.translate = camera_.translate + shakeOffset;
}

void CameraManager::Shake()
{
	// シェイクの適用
	if (shakeTimeElapsed_ < shakeDuration_) {
		ApplyShake();
		shakeTimeElapsed_ += deltaTime_;
	}
	else {
		shakeIntensity_ = 0.0f;
		shakeDuration_ = 0.0f;
		shakeTimeElapsed_ = 0.0f;
	}
}


