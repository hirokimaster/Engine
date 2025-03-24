#pragma once
#include "application/GameObject/CameraManager/FollowCamera/FollowCamera.h"

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

private:
	CameraManager() = default;
	~CameraManager() = default;
	CameraManager(const CameraManager&) = delete;
	const CameraManager& operator=(const CameraManager&) = delete;
};
