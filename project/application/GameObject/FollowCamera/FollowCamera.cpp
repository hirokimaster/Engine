/**
* @file FollowCamera.cpp
* @brief 追従カメラ
* @author 仁平 琉乃
*/

#include "FollowCamera.h"

void FollowCamera::Initialize()
{
	camera_.Initialize();
	// シェイク
	shakeIntensity_ = 0.0f;
	shakeDuration_ = 0.0f;
	shakeTimeElapsed_ = 0.0f;
}

void FollowCamera::Update()
{
	// 追従する対象がいたら
	if (target_) {
		// 追従
		camera_.translate = target_->translate + offset_;

		// targrtの回転によって動かす
		float cameraRotateZ = target_->rotate.z;
		cameraRotateZ = std::clamp(cameraRotateZ, -0.2f, 0.2f);
		camera_.rotate.z = cameraRotateZ;

		// カメラの行列を更新
		camera_.UpdateMatrix();
	}


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

void FollowCamera::StartShake(float intensity, float duration)
{
	shakeIntensity_ = intensity;
	shakeDuration_ = duration;
	shakeTimeElapsed_ = 0.0f;
}

void FollowCamera::ApplyShake()
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
