/**
* @file BulletTrajectory.h
* @brief 弾の軌跡
* @author 仁平 琉乃
*/

#pragma once
#include "engine/3d/BaseObject/BaseInstancingObject.h"

class BulletTrajectory{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update(const Vector3& bullletPosition);

private:
	std::queue<std::weak_ptr<Object3dInstancing>> pool_;
	const uint32_t kMaxTrailPoints = 10000;
};
