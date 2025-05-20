/**
* @file CollisionManager.h
* @brief 衝突判定を管理するクラス
* @author 仁平 琉乃
*/

#pragma once
#include "engine/Utility/CollisionManager/Collider/Collider.h"
#include "engine/Math/Mathfunction.h"
#include <algorithm>
#include <unordered_map>

class CollisionManager {
public:
	/// <summary>
	/// 当たり判定チェック
	/// </summary>
	void CheckAllCollision();

	/// <summary>
	/// コライダーリストに追加
	/// </summary>
	/// <param name="collider"></param>
	void ColliderPush(Collider* collider) { colliders_.push_back(collider); }

	/// <summary>
	/// コライダーリストをクリア
	/// </summary>
	void ColliderClear() { colliders_.clear(); }

private:
	/// <summary>
	/// 衝突フィルタリング
	/// </summary>
	/// <param name="cA"></param>
	/// <param name="cB"></param>
	void CheckCollisionPair(Collider* cA, Collider* cB);

	/// <summary>
	/// 球同士の当たり判定
	/// </summary>
	/// <param name="preV1"></param>
	/// <param name="currV1"></param>
	/// <param name="v1Radious"></param>
	/// <param name="preV2"></param>
	/// <param name="currV2"></param>
	/// <param name="v2Radious"></param>
	/// <returns></returns>
	bool CheckCollision(Vector3 preV1,Vector3 currV1, float v1Radious, Vector3 preV2,Vector3 currV2, float v2Radious);

	/// <summary>
	/// aabbと球の当たり判定
	/// </summary>
	/// <param name="aabb"></param>
	/// <param name="center"></param>
	/// <param name="radious"></param>
	/// <returns></returns>
	bool CheckCollision(const AABB& aabb, const Vector3 center,float radius);

private:

	std::unordered_map<Collider*, Vector3> prevPositions_;
	std::list<Collider*> colliders_;
};
