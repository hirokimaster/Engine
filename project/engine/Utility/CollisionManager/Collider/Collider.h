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

enum class ColliderType {
	None,
	Sphere,
	AABB,
	OBB
};

class Collider {
public:

	/// <summary>
	/// 当たった時
	/// </summary>
	bool OnCollision() { return onCollision_; }
	void SetOnCollision(bool onCollision) { onCollision_ = onCollision; }

#pragma region getter

	float GetRadious() { return radious_; }
	const Vector3& GetWorldPosition() const { return worldPosition_; }
	const Vector3& GetScale() const { return scale_; }
	uint32_t GetCollosionAttribute() const { return collisionAttribute_; }
	uint32_t GetCollisionMask() const { return CollisionMask_; }
	ColliderType GetType()const { return type_; }

#pragma endregion

#pragma region setter

	void SetRadious(float radious) { radious_ = radious; }
	void SetCollosionAttribute(uint32_t collisionAttribute) { collisionAttribute_ = collisionAttribute; }
	void SetCollisionMask(uint32_t collisionMask) { CollisionMask_ = collisionMask; }
	void SetType(ColliderType type) { type_ = type; }
	void SetWorldPosition(const Vector3& worldPosition) { worldPosition_ = worldPosition; }
	void SetScale(const Vector3& scale) { scale_ = scale; }

#pragma endregion

private:

	// 自機
	uint32_t collisionAttribute_ = 0xffffffff;
	// 相手
	uint32_t CollisionMask_ = 0xffffffff;

	float radious_ = 1.0f;
	
	ColliderType type_ = ColliderType::Sphere;
	// スケール
	Vector3 scale_{};
	// ワールド座標
	Vector3 worldPosition_{};

	bool onCollision_ = false;
};