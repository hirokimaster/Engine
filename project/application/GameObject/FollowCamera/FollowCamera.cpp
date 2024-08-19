#include "FollowCamera.h"

void FollowCamera::Initialize()
{
	camera_.Initialize();
}

void FollowCamera::Update()
{
	// 追従する対象がいたら
	if (target_) {

		// 追従対象からカメラまでのオフセット
		Vector3 offset = { 0.0f,2.0f,-60.0f };

		// 座標をコピーしてオフセット分ずらす
		camera_.translate = Add(target_->translate, offset);

		camera_.UpdateMatrix();
	}


}

void FollowCamera::Rotate()
{
	XINPUT_STATE joyState;
	if (Input::GetInstance()->GetJoystickState(joyState)) {
		float kRotateSpeed = 0.03f;
		camera_.rotate.y += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * kRotateSpeed;
	}

}
