/**
* @file FollowCamera.cpp
* @brief 追従カメラ
* @author 仁平 琉乃
*/

#include "FollowCamera.h"

void FollowCamera::Initialize()
{
	
}

void FollowCamera::Update(Camera& camera)
{
	// 追従する対象がいたら
	if (target_) {
		// 追従
		camera.translate = target_->translate + offset_;

		// targrtの回転によって動かす
		float cameraRotateZ = target_->rotate.z;
		cameraRotateZ = std::clamp(cameraRotateZ, -0.2f, 0.2f);
		camera.rotate.z = cameraRotateZ;

		camera.UpdateMatrix();
	}

	camera.UpdateMatrix();
}