/**
* @file FollowCamera.h
* @brief 追従カメラ
* @author 仁平 琉乃
*/

#pragma once
#include "engine/Graphics/Transform/WorldTransform.h"
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
	void Update(Camera& camera);

public:


#pragma region setter

	void SetTarget(const WorldTransform* target) { target_ = target; }

	void SetLockOn(LockOn* lockOn) { lockOn_ = lockOn; }

	void SetOffset(const Vector3& offset) { offset_ = offset; }

#pragma endregion

private:
	const WorldTransform* target_ = nullptr;
	LockOn* lockOn_ = nullptr;
	Vector3 offset_{};
};
