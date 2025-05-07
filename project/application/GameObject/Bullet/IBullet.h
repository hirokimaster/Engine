#pragma once
#include "engine/3d/BaseObject/BaseInstancingObject.h"

// 弾のタイプ
enum class BulletType {
	Player,
	Enemy
};

class IBullet : public BaseInstancingObject {
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
	/// 移動
	/// </summary>
	virtual void Move() = 0;

	/// <summary>
	/// 消滅時間のリセット
	/// </summary>
	virtual void ResetDeathTimer() = 0;

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

	/// <summary>
	/// 弾の種類取得
	/// </summary>
	/// <returns></returns>
	virtual BulletType GetBulletType() = 0;

	/// <summary>
	/// アクティブかどうか
	/// </summary>
	/// <returns></returns>
	virtual bool GetIsActive()const = 0;

#pragma endregion

#pragma region setter

	virtual void SetIsActive(bool isActive) = 0;

	virtual void SetIsDead(bool isDead) = 0;

#pragma endregion
};
