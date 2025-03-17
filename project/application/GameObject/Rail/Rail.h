/**
* @file Rail.h
* @brief レールを生成
* @author 仁平 琉乃
*/

#pragma once
#include "engine/Graphics/Transform/WorldTransform.h"
#include "engine/Utility/ImGuiManager/ImGuiManager.h"
#include <algorithm>
#include "application/GameObject/LockOn/LockOn.h"
#include "application/GameObject/Player/Player.h"

class Rail {
public:

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

#pragma region setter

	void SetPlayer(Player* player) { player_ = player; }

	void SetControlPoints(std::vector<Vector3> points) { controlPoints_ = points; }

#pragma endregion

private:
	// レール用の制御点
	std::vector<Vector3> controlPoints_{};
	// playerのポインタ借りてくる
	Player* player_ = nullptr;
	float t_ = 0.0f;

};
