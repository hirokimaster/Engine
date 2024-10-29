/**
* @file FollowCamera.h
* @brief 追従カメラ
* @author 仁平 琉乃
*/

#pragma once
#include "engine/Transform/WorldTransform.h"
#include "engine/Utility/ImGuiManager/ImGuiManager.h"
#include <algorithm>
#include "application/GameObject/LockOn/LockOn.h"
#include <random>

class FollowCamera {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// シェイク開始
	/// </summary>
	/// <param name="intensity"></param>
	/// <param name="duration"></param>
	void StartShake(float intensity, float duration);

private:

	/// <summary>
	/// シェイク適用
	/// </summary>
	void ApplyShake();

public:


#pragma region setter

	void SetTarget(const WorldTransform* target) { target_ = target; }

	void SetLockOn(LockOn* lockOn) { lockOn_ = lockOn; }

	void SetRotate(const Vector3& rotate) { camera_.rotate = rotate; }

	void SetOffset(const Vector3& offset) { offset_ = offset; }

#pragma endregion

#pragma region getter

	Camera GetCamera() { return camera_; }

#pragma endregion

private:
	Camera camera_;
	const WorldTransform* target_ = nullptr;
	LockOn* lockOn_ = nullptr;
	Vector3 offset_{};

	float shakeIntensity_ = 0.0f;
	float shakeDuration_ = 0.0f;
	float shakeTimeElapsed_ = 0.0f;
	const float deltaTime_ = 1.0f / 60.0f;
};
