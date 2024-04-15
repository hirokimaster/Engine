#pragma once
#include "engine/Utility/CollisionManager/Collider/Collider.h"
#include "engine/Math/Mathfunction.h"

class CollisionManager {
public:
	void CheckAllCollision();

	void ColliderPush(Collider* collider) { colliders_.push_back(collider); }
	void ColliderClear() { colliders_.clear(); }

private:
	void CheckCollisionPair(Collider* cA, Collider* cB);
	bool CheckBallCollision(Vector3 v1, float v1Radious, Vector3 v2, float v2Radious);

	std::list<Collider*> colliders_;
};
