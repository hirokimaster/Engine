/**
* @file CameraManager.h
* @brief ゲーム中のカメラのマネージャ
* @author 仁平 琉乃
*/

#pragma once
#include "application/GameObject/CameraManager/FollowCamera/FollowCamera.h"
#include "engine/Utility/Random/Random.h"

class CameraManager {
public:
	/// <summary>
	/// シングルトンにする
	/// </summary>
	/// <returns></returns>
	static CameraManager* GetInstance();

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

	/// <summary>
	/// シェイク
	/// </summary>
	void Shake();

public:

#pragma region getter

	const Camera& GetCamera()const { return camera_; }

	const std::unique_ptr<FollowCamera>& GetFollowCamera()const { return followCamera_; }

#pragma endregion

#pragma region setter

	void SetCameraRotateZ(float rotate) { camera_.rotate.z = rotate; }

	void SetCameraRotate(const Vector3& rotate) { camera_.rotate = rotate; }

	void SetCameraTranslate(const Vector3& translate) { camera_.translate = translate; }

#pragma endregion

private:
	// ノーマルカメラ
	Camera camera_{};
	// 追従カメラ
	std::unique_ptr<FollowCamera> followCamera_ = nullptr;

	float shakeIntensity_ = 0.0f;
	float shakeDuration_ = 0.0f;
	float shakeTimeElapsed_ = 0.0f;
	const float deltaTime_ = 1.0f / 60.0f;

private:
	CameraManager() = default;
	~CameraManager() = default;
	CameraManager(const CameraManager&) = delete;
	const CameraManager& operator=(const CameraManager&) = delete;
};
