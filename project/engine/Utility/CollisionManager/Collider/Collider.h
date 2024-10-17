/**
* @file Collider.h
* @brief 衝突判定を行いたいクラスに継承させる
* @author 仁平 琉乃
*/

#pragma once
#include "engine/Math/Vector3.h"
#include <stdint.h>
#include <list>
#include "ColliderConfig.h"

class Collider {
public:

	/// <summary>
	/// 当たった時
	/// </summary>
	virtual void OnCollision() = 0;

#pragma region getter

	float GetRadious() { return radious_; }
    virtual Vector3 GetWorldPosition() const = 0;
	uint32_t GetCollosionAttribute() const { return collisionAttribute_; }
	uint32_t GetCollisionMask() const { return CollisionMask_; }

#pragma endregion

#pragma region setter

	void SetRadious(float radious) { radious_ = radious; }
	void SetCollosionAttribute(uint32_t collisionAttribute) { collisionAttribute_ = collisionAttribute; }
	void SetCollisionMask(uint32_t collisionMask) { CollisionMask_ = collisionMask; }

#pragma endregion

private:

	// 自機
	uint32_t collisionAttribute_ = 0xffffffff;
	// 相手
	uint32_t CollisionMask_ = 0xffffffff;

	float radious_ = 1.0f;
};