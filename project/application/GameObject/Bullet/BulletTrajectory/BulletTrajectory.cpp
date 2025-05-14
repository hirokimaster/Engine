#include "BulletTrajectory.h"
/**
* @file BulletTrajectory.cpp
* @brief 弾の軌跡
* @author 仁平 琉乃
*/

void BulletTrajectory::Initialize()
{
	// object共通の初期化
	BaseInstancingObject::Initialize("Player/plane.obj", "TempTexture/noise0.png", ColliderType::AABB);
}

void BulletTrajectory::Update()
{
}
