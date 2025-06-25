/**
* @file PlaneProjectionShadow.cpp
* @brief 影
* @author 仁平 琉乃
*/

#include "PlaneProjectionShadow.h"

void PlaneProjectionShadow::Initialize(const std::string& modelName, const WorldTransform* casterWorldTransform)
{
	// 共通処理初期化
	TextureManager::Load("resources/TempTexture/uvChecker.png");
    BaseInstancingObject::Initialize(modelName, "TempTexture/uvChecker.png", ColliderType::None, true);

	// 平面投影行列
	shadowMatrix_ = MakeIdentityMatrix();
	shadowMatrix_.m[1][1] = 0.002f;
	casterWorldTransform_ = casterWorldTransform;
	// objectを真っ黒にする
	object_.lock()->color = {0.0f,0.0f,0.0f,1.0f};
}

void PlaneProjectionShadow::Update()
{
  
    auto obj = object_.lock();
    if (!obj || !casterWorldTransform_) return;

    // 影の元となるワールド行列
    Matrix4x4 mat = casterWorldTransform_->matWorld;

    // 影にする（Y方向を潰す）
    Matrix4x4 shadow = MakeIdentityMatrix();
    shadow.m[1][1] = 0.002f;

    // 拡大（XZ方向のみ）
    Matrix4x4 scale = MakeScaleMatrix({ 5.0f, 1.0f, 5.0f });

    // 平行移動
    Matrix4x4 offset = MakeTranslateMatrix(offset_);

    // 計算順：スケーリングとオフセットは左から
    Matrix4x4 matWorld = scale * shadow * mat;
    matWorld = offset * matWorld;

    obj->worldTransform.matWorld = matWorld;


    // ImGui UI表示
    ImGui::Begin("offset");
    ImGui::DragFloat3("off", &offset_.x, 0.1f, -1000.0f, 1000.0f);
    ImGui::Text("x[%.2f], y[%.2f], z[%.2f]",
        obj->worldTransform.matWorld.m[3][0],
        obj->worldTransform.matWorld.m[3][1],
        obj->worldTransform.matWorld.m[3][2]);
    ImGui::End();
}

