/**
* @file BulletTrajectory.h
* @brief 弾の軌跡
* @author 仁平 琉乃
*/

#pragma once
#include "engine/3d/BaseObject/BaseInstancingObject.h"

class BulletTrajectory : public BaseInstancingObject{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

private:

};
