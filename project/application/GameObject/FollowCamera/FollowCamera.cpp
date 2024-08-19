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
		Vector3 offset = { 0.0f,2.0f,-80.0f };

		//カメラの角度から回転行列を計算
		Matrix4x4 rotateMatrix = MakeRotateMatrix(camera_.rotate);
		// オフセットをカメラの回転に合わせて回転
		offset = TransformNormal(offset, rotateMatrix);

		// 座標をコピーしてオフセット分ずらす
		camera_.translate = Add(target_->translate, offset);

	}

	XINPUT_STATE joyState;
	if (Input::GetInstance()->GetJoystickState(joyState)) {
		float kRotateSpeed = 0.0001f;
		camera_.rotate.y += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * kRotateSpeed;
	}

	camera_.UpdateMatrix();
}