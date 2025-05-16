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
				"Player/ball.obj",
				TextureManager::GetTexHandle("TempTexture/noise0.png")
			);

		if (auto ptr = obj.lock()) {
			ptr->isAlive = false;
			pool_.push(obj);
		}
	}
}

void BulletTrajectory::Update()
{
	Fade();
}

void BulletTrajectory::AddTrailPoint(const Vector3& position)
{
	if (pool_.empty()) return;

	std::weak_ptr<Object3dInstancing> obj = pool_.front();
	pool_.pop();

	// 煙の揺らぎ
	Vector3 offset = {
	    Random::Range(-0.08f, 0.08f),
	    Random::Range(0.0f, 0.08f),
	    Random::Range(-0.08f, 0.08f)
	};

	if (auto param = obj.lock()) {
		param->isAlive = true;
		param->worldTransform.translate = position + offset;
		param->worldTransform.scale = { 1.0f, 1.0f, 50.0f };
		param->color.w = 1.0f;

		activeTrails_.push_back(obj); // 使用中リストに入れる
	}
}

void BulletTrajectory::Fade()
{
	std::vector<std::weak_ptr<Object3dInstancing>> updatedActive;

	for (auto& obj : activeTrails_) {
		if (auto param = obj.lock()) {
			if (!param->isAlive) continue;

			// だんだん透明に
			param->color.w -= 0.08f;

			// だんだん大きくする
			param->worldTransform.scale = param->worldTransform.scale + Vector3(0.1f, 0.1f, 0.1f);

			// 少し上昇
			param->worldTransform.translate.y += 0.01f;

			if (param->color.w <= 0.0f) {
				param->isAlive = false;
				param->color.w = 0.0f;

				// スケール初期化
				param->worldTransform.scale = { 1.0f, 1.0f, 50.0f };

				// プールに戻す
				pool_.push(obj);
				continue;
			}

			updatedActive.push_back(obj);
		}
	}

	activeTrails_ = std::move(updatedActive);
}

