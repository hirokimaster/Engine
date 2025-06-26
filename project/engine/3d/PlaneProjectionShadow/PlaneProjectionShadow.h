#pragma once
#include "engine/3d/BaseObject/BaseInstancingObject.h"
#include <variant>

/**
* @file PlaneProjectionShadow.h
* @brief 影
* @author 仁平 琉乃
*/

template<typename T>
class PlaneProjectionShadow : public BaseInstancingObject
{
public:
    /// <summary>
    /// 初期化
    /// </summary>
    void Initialize(const std::string& modelName, const T* casterWorldTransform);

    /// <summary>
    /// 更新
    /// </summary>
    void Update();

    void SetOffset(const Vector3& offset) { offset_ = offset; }
    void SetScale(const Vector3& scale) { scale_ = scale; }
    void SetIsActive(bool flag) { object_.lock()->isAlive = flag; }

private:
    // 投影元オブジェクトのワールド変換データ
    const T* casterWorldTransform_ = nullptr;
    // 平面投影行列
    Matrix4x4 shadowMatrix_;
    // オフセット 
    Vector3 offset_{};
    // scale
    Vector3 scale_{};
};

