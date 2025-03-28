#pragma once
#include "engine/Graphics/Camera/Camera.h"
#include "engine/3d/Object3DPlacer/BaseObject.h"
#include "application/GameObject/Bullet/EnemyBullet/EnemyBullet.h"

class IEnemy : public BaseObject{
public:

	/// <summary>
	/// 仮想デストラクタ
	/// </summary>
	virtual ~IEnemy() = default;
	
	/// <summary>
	/// 攻撃
	/// </summary>
	virtual void Fire() = 0;

	/// <summary>
	/// worldPosition取得
	/// </summary>
	/// <returns></returns>
	virtual Vector3 GetWorldPosition()const = 0;

	/// <summary>
	/// デスフラグ取得
	/// </summary>
	/// <returns></returns>
	virtual bool GetIsDead()const = 0;

	/// <summary>
	/// コライダーのポインタ取得
	/// </summary>
	/// <returns></returns>
	virtual Collider* GetCollider() = 0;
};
