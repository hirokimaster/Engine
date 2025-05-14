#include "BulletTrajectory.h"
/**
* @file BulletTrajectory.cpp
* @brief 弾の軌跡
* @author 仁平 琉乃
*/

void BulletTrajectory::Initialize()
{
    //  あらかじめpoolに作っておく
    for (uint32_t i = 0; i < kMaxTrailPoints; ++i) {
        std::weak_ptr<Object3dInstancing> obj =
            ObjectManager::GetInstance()->CreateInstancingObject(
                "Player/player.obj",
                TextureManager::GetTexHandle("TempTexture/white.png")
            );

        obj.lock()->isAlive = false;
        obj.lock()->color.w = 1.0f;

        if (obj.lock()) {
            pool_.push(obj);
        }
    }
}

void BulletTrajectory::Update(const Vector3& bullletPosition)
{
    if (pool_.empty())return; // 空なら早期

    std::weak_ptr<Object3dInstancing> obj = pool_.front();
    pool_.pop();
    if (auto param = obj.lock()) {
        param->isAlive = true;
        param->worldTransform.translate = (bullletPosition);
        param->worldTransform.scale = { 1.0f,1.0f,1.0f };

        // alpha値をだんだん下げて0になったらactiveを外す
        param->color.w -= 0.01f;
        if (param->color.w <= 0.0f) {
            param->isAlive = false;
            param->color.w = 1.0f;
            pool_.push(obj);
        }
    }

}

