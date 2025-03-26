#pragma once
#include "engine/Graphics/Camera/Camera.h"
#include "engine/3d/Object3DPlacer/BaseObject.h"

class IEnemy : public BaseObject{
public:
	
	/// <summary>
	/// 攻撃
	/// </summary>
	virtual void Fire() = 0;

	/// <summary>
	/// worldPosition取得
	/// </summary>
	/// <returns></returns>
	virtual Vector3 GetWorldPosition()const = 0;

};
