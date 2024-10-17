#pragma once
#include "engine/Transform/WorldTransform.h"
#include "engine/Utility/ImGuiManager/ImGuiManager.h"
#include <algorithm>
#include "application/GameObject/LockOn/LockOn.h"
#include "application/GameObject/Player/Player.h"

class Rail {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

#pragma region setter

	void SetPlayer(Player* player) { player_ = player; }

#pragma endregion

private:

	/// <summary>
	/// レールカメラを動かす計算をする所
	/// </summary>
	void RailPositionCalc(float progress);
	
	/// <summary>
	/// catmulrom補間
	/// </summary>
	/// <param name="p0"></param>
	/// <param name="p1"></param>
	/// <param name="p2"></param>
	/// <param name="p3"></param>
	/// <param name="t"></param>
	/// <returns></returns>
	Vector3 CatmullRomInterpolation(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, float t);

	/// <summary>
	/// catmulrom補間
	/// </summary>
	/// <param name="points"></param>
	/// <param name="t"></param>
	/// <returns></returns>
	Vector3 CatmullRomPosition(const std::vector<Vector3>& points, float t);

	/// <summary>
	/// 進行状況の計算
	/// </summary>
	/// <param name="time"></param>
	/// <param name="speed"></param>
	/// <returns></returns>
	float ProgressCalc(float time, float speed);

private:
	// レール用の制御点
	std::vector<Vector3> controlPoints_{};
	// playerのポインタ借りてくる
	Player* player_ = nullptr;
	float deltaTime_ = 0.0f;
};
