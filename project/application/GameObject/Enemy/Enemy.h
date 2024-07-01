#pragma once
#include "application/GameObject/BaseObject/BaseObject.h"
#include "engine/Utility/CollisionManager/Collider/Collider.h"

class Enemy : public BaseObject, public Collider {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="object"></param>
	/// <param name="texHandle"></param>
	/// <param name="model"></param>
	void Initialize(Object3DPlacer* object, uint32_t texHandle, const std::string& model)override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	void Draw(Camera& camera)override;

private:

	/// <summary>
	/// 当たり判定
	/// </summary>
	void OnCollision()override;

	/// <summary>
	/// 死んだ
	/// </summary>
	void Dead();

public:

#pragma region getter

	Vector3 GetWorldPosition()override;

	bool GetIsDead() { return isDead_; }
	

#pragma endregion

private:

	bool isDead_ = false; // デスフラグ
};