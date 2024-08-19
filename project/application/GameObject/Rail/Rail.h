#pragma once
#include "engine/Transform/WorldTransform.h"
#include "engine/Utility/ImGuiManager/ImGuiManager.h"
#include <algorithm>
#include "application/GameObject/LockOn/LockOn.h"

class Rail {
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
	/// worldTransform取得
	/// </summary>
	const WorldTransform& GetWorldTransform() { return worldTransform_; }

	Vector3 GetWorldPosition() const;

#pragma region setter

	void SetRotate(Vector3 rotate) { worldTransform_.rotate = rotate; }

	void SetPosition(Vector2 position) { worldTransform_.translate.x = position.x, worldTransform_.translate.y = position.y; }

#pragma endregion

private:

	/// <summary>
	/// レールカメラを動かす計算をする所
	/// </summary>
	void RailPositionCalc();
	
	Vector3 CatmullRomInterpolation(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, float t);

	Vector3 CatmullRomPosition(const std::vector<Vector3>& points, float t);

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// 移動速度
	Vector3 velocity_ = { 0.0f, 0.0f, 0.0f };
};
