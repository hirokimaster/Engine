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

	// Colliderの現在位置を、次フレーム用に保存
	for (Collider* collider : colliders_) {
		prevPositions_[collider] = collider->GetWorldPosition();
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
		Vector3 prevPosA = prevPositions_[cA];
		Vector3 currPosA = cA->GetWorldPosition();

		Vector3 prevPosB = prevPositions_[cB];
		Vector3 currPosB = cB->GetWorldPosition();

		float cARadious = cA->GetRadious();
		float cBRadious = cB->GetRadious();

		if (CheckCollision(prevPosA, currPosA, cARadious, prevPosB, currPosB, cBRadious)) {
			cA->SetOnCollision(true);
			cB->SetOnCollision(true);
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
			cA->SetOnCollision(true);
			cB->SetOnCollision(true);
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
			cA->SetOnCollision(true);
			cB->SetOnCollision(true);
		}
	}																			  
}



bool CollisionManager::CheckCollision(Vector3 preV1, Vector3 currV1, float v1Radious, Vector3 preV2, Vector3 currV2, float v2Radious)
{
	// 移動ベクトルの差
	Vector3 moveA = Subtract(currV1, preV1);
	Vector3 moveB = Subtract(currV2, preV2);
	Vector3 relativeMove = Subtract(moveB, moveA);

	// 出発点の差
	Vector3 start = Subtract(preV1, preV2);

	float combinedRadius = v1Radious + v2Radious;

	// 2次方程式の係数を計算
	float a = Dot(relativeMove, relativeMove);
	float b = 2.0f * Dot(start, relativeMove);
	float c = Dot(start, start) - (combinedRadius * combinedRadius);

	float discriminant = b * b - 4 * a * c;
	if (discriminant < 0.0f) {
		return false; // 交差しない
	}

	// 衝突する時間t（0 <= t <= 1 であればこのフレーム中に衝突）
	float t = (-b - sqrtf(discriminant)) / (2.0f * a);
	return (t >= 0.0f && t <= 1.0f);
}

bool CollisionManager::CheckCollision(const AABB& aabb, const Vector3 center,float radius)
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
