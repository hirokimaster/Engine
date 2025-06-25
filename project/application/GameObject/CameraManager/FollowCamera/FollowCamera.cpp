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
    if (target_ && lockOn_) {
        // プレイヤー（自機）のワールド座標
        Vector3 playerPos = target_->translate;
        // レティクルのワールド座標
        Vector3 reticlePos = lockOn_->GetWorldPosition3DReticle();

        // カメラの理想位置（自機の後方・上方）
        Vector3 idealCameraPos = playerPos + offset_;

        // 注視点は自機とレティクルの中間
        Vector3 idealLookAt = 0.5f * (playerPos + reticlePos);

        // 現在位置から理想位置へLerpで追従
        float followSpeed = 0.5f;
        camera.translate = camera.translate + followSpeed * (idealCameraPos - camera.translate);
        camera.rotate = camera.rotate + followSpeed * (target_->rotate - camera.rotate);

        // カメラの注視点を更新
        camera.matView = MakeViewMatrix(camera.translate, idealLookAt, Vector3(0, 1, 0));

        camera.UpdateMatrix();
    } else if (target_) {
        // lockOn_が無い場合は従来通り
        camera.translate = target_->translate + offset_;
        float cameraRotateZ = target_->rotate.z;
        cameraRotateZ = std::clamp(cameraRotateZ, -0.2f, 0.2f);
        camera.rotate.z = cameraRotateZ;
        camera.UpdateMatrix();
    } else {
        camera.UpdateMatrix();
    }
}