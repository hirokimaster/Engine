#pragma once
#include "engine/Transform/WorldTransform.h"
#include "engine/Utility/ImGuiManager/ImGuiManager.h"
#include <algorithm>

class RailCamera {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const Vector3& position, const Vector3& rotate);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// camera取得
	/// </summary>
	const Camera& GetCamera() { return camera_; }

	/// <summary>
	/// worldTransform取得
	/// </summary>
	const WorldTransform& GetWorldTransform() { return worldTransform_; }

private:

	/// <summary>
	/// レールカメラを動かす計算をする所
	/// </summary>
	void MoveOnRail();

	Vector3 CatmullRomInterpolation(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, float t);

	Vector3 CatmullRomPosition(const std::vector<Vector3>& points, float t);

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// ビュープロジェクション
	Camera camera_;
	// 移動速度
	Vector3 velocity_ = { 0.0f, 0.0f, 0.0f };
	// 角度
	Vector3 rotate_ = { 0.0f, 0.0f, 0.0f };
};
