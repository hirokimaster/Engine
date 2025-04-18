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
	std::random_device rd;
	std::mt19937 seed(rd());
	std::uniform_real_distribution<float> dist(-8.0f, 8.0f);

	// ランダムな値を生成してカメラの位置に加算
	Vector3 shakeOffset(
		dist(seed) * shakeIntensity_,
		dist(seed) * shakeIntensity_,
		dist(seed) * shakeIntensity_
	);

	camera_.translate = camera_.translate + shakeOffset;
}


