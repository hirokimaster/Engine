/**
* @file BulletTrajectory.h
* @brief 弾の軌跡
* @author 仁平 琉乃
*/

#pragma once
#include "engine/3d/BaseObject/BaseInstancingObject.h"
#include "engine/Utility/Random/Random.h"

class BulletTrajectory{
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
	/// poolから取り出す
	/// </summary>
	/// <param name="position"></param>
	void AddTrailPoint(const Vector3& position);

private:

	/// <summary>
	/// だんだん透明にする
	/// </summary>
	void Fade();

private:
	std::queue<std::weak_ptr<Object3dInstancing>> pool_;
	std::vector<std::weak_ptr<Object3dInstancing>> activeTrails_;
	const uint32_t kMaxTrailPoints = 1000;
};
