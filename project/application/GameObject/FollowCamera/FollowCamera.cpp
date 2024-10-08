#include "FollowCamera.h"

void FollowCamera::Initialize()
{
	camera_.Initialize();
}

void FollowCamera::Update()
{
	// 追従する対象がいたら
	if (target_) {

		camera_.translate = target_->translate + offset_;

		// カメラの行列を更新
		camera_.UpdateMatrix();
	}


}

void FollowCamera::Rotate()
{
	
}
