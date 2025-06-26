/**
* @file PlaneProjectionShadow.cpp
* @brief 影
* @author 仁平 琉乃
*/

#include "PlaneProjectionShadow.h"

template<typename T>
void PlaneProjectionShadow<T>::Initialize(const std::string& modelName, const T* casterWorldTransform)
{
    // 共通処理初期化
    TextureManager::Load("resources/TempTexture/uvChecker.png");
    BaseInstancingObject::Initialize(modelName, "TempTexture/uvChecker.png", ColliderType::None, true);

    // 平面投影行列
    shadowMatrix_ = MakeIdentityMatrix();
    shadowMatrix_.m[1][1] = 0.002f;
    casterWorldTransform_ = casterWorldTransform;
    // objectを真っ黒にする
    object_.lock()->color = { 0.0f,0.0f,0.0f,1.0f };
}

template<typename T>
void PlaneProjectionShadow<T>::Update()
{
    auto obj = object_.lock();
    if (!obj || !casterWorldTransform_) return;

    // 影の元となるワールド行列
    Matrix4x4 mat = casterWorldTransform_->matWorld;

    // 影にする（Y方向を潰す）
    Matrix4x4 shadow = MakeIdentityMatrix();
    shadow.m[1][1] = 0.002f;

    // 拡大（XZ方向のみ）
    Matrix4x4 scale = MakeScaleMatrix({ scale_.x, scale_.y, scale_.z });

    // 計算順：スケーリングとオフセットは左から
    Matrix4x4 matWorld = scale * shadow * mat;

    // 最後にoffset_を直接加算
    matWorld.m[3][0] += offset_.x;
    matWorld.m[3][1] += offset_.y;
    matWorld.m[3][2] += offset_.z;

    obj->worldTransform.matWorld = matWorld;
}

// 明示的インスタンス化
template class PlaneProjectionShadow<WorldTransform>;
template class PlaneProjectionShadow<InstanceWorldTransform>;

