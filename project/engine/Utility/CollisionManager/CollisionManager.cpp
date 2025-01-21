/**
* @file CollisionManager.cpp
* @brief 衝突判定を管理するクラス
* @author 仁平 琉乃
*/

#include "CollisionManager.h"

void CollisionManager::CheckAllCollision() {

	std::list<Collider*>::iterator itrA = colliders_.begin();

	for (; itrA != colliders_.end(); ++itrA) {

		Collider* colliderA = *itrA;

		std::list<Collider*>::iterator itrB = itrA;
		itrB++;
		for (; itrB != colliders_.end(); ++itrB) {
			Collider* colliderB = *itrB;
			//当たり判定処理
			CheckCollisionPair(colliderA, colliderB);
		}
	}
}

void CollisionManager::CheckCollisionPair(Collider* cA, Collider* cB) {

	//フィルタリング
	if ((cA->GetCollosionAttribute() & cB->GetCollisionMask()) == 0 ||
		(cA->GetCollisionMask() & cB->GetCollosionAttribute()) == 0) {
		return;
	}

	if (cA->GetType() == ColliderType::Sphere && cB->GetType() == ColliderType::Sphere) {
		//当たり判定の計算開始
		Vector3 cApos = cA->GetWorldPosition();
		Vector3 cBpos = cB->GetWorldPosition();

		float cARadious = cA->GetRadious();
		float cBRadious = cB->GetRadious();

		if (CheckCollision(cApos, cARadious, cBpos, cBRadious)) {
			cA->OnCollision();
			cB->OnCollision();
		}

	}
	else if (cA->GetType() == ColliderType::AABB && cB->GetType() == ColliderType::Sphere) {
		// 当たり判定の計算
		Vector3 cApos = cA->GetWorldPosition();
		Vector3 cAScale = cA->GetScale();
		Vector3 cBpos = cB->GetWorldPosition();

		float cBRadius = cB->GetRadious();

		AABB aabb{};
		aabb.min = cApos - cAScale;
		aabb.max = cApos + cAScale;

		if (CheckCollision(aabb, cBpos, cBRadius)) {
			cA->OnCollision();
			cB->OnCollision();
		}
	}
	else if (cA->GetType() == ColliderType::Sphere && cB->GetType() == ColliderType::AABB) {
		// 当たり判定の計算
		Vector3 cApos = cA->GetWorldPosition();
		Vector3 cBpos = cB->GetWorldPosition();
		Vector3 cBScale = cB->GetScale();

		float cARadius = cA->GetRadious();

		AABB aabb{};
		aabb.min = cBpos - cBScale;
		aabb.max = cBpos + cBScale;

		if (CheckCollision(aabb, cApos, cARadius)) {
			cA->OnCollision();
			cB->OnCollision();
		}
	}																			  
}

bool CollisionManager::CheckCollision(Vector3 v1, float v1Radious, Vector3 v2, float v2Radious)
{
	float x = (v2.x - v1.x);
	float y = (v2.y - v1.y);
	float z = (v2.z - v1.z);

	float position = (x * x) + (y * y) + (z * z);

	float radious = v1Radious + v2Radious;


	if (position <= (radious * radious)) {
		return true;
	}
	else {
		return false;
	}


}

bool CollisionManager::CheckCollision(const AABB& aabb, const Vector3 center, const float radius)
{
	Vector3 closesetPoint{
		std::clamp(center.x, aabb.min.x, aabb.max.x), std::clamp(center.y, aabb.min.y, aabb.max.y),
		std::clamp(center.z, aabb.min.z, aabb.max.z) };
	float distance = Length(Subtract(closesetPoint, center));

	if (distance <= radius) {

		return true;
	}

	return false;
}
