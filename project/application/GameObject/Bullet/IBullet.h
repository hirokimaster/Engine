#pragma once
#include "engine/3d/Object3DPlacer/BaseObject.h"

class IBullet : public BaseObject {
public:
	/// <summary>
	/// 仮想デストラクタ
	/// </summary>
	virtual ~IBullet() = default;

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
	virtual void Draw() = 0;

	/// <summary>
	/// 移動
	/// </summary>
	virtual void Move() = 0;

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
