#include "FollowCamera.h"

void FollowCamera::Initialize()
{
	camera_.Initialize();
}

void FollowCamera::Update()
{
	// 追従する対象がいたら
	if (target_) {

		// オフセット
		Vector3 offset = { 0.0f, 2.0f, -60.0f };

		camera_.translate = target_->translate + offset;

		// カメラの行列を更新
		camera_.UpdateMatrix();
	}


}

void FollowCamera::Rotate()
{
	
}
