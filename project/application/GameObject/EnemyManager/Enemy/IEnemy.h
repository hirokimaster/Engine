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
	/// 初期化
	/// </summary>
	virtual void Initialize() = 0;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	virtual void Draw(const Camera& camera) = 0;
	
	/// <summary>
	/// 攻撃
	/// </summary>
	virtual void Fire() = 0;

#pragma region setter

	virtual void SetPosition(const Vector3& position) = 0;

#pragma endregion

#pragma region getter

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

#pragma endregion
};
