#pragma once

#pragma once
#include "engine/3d/Object3DPlacer/BaseObject.h"
#include "engine/Utility/CollisionManager/Collider/Collider.h"
#include "engine/Graphics/TextureManager/TextureManager.h"

class Wall : public BaseObject {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	void Draw(const Camera& camera);

#pragma region getter

	bool GetIsHit()const { return isHit_; }

	Vector3 GetWorldPosition()const;

	Collider* GetCollider()override { return collider_.get(); }

#pragma endregion

#pragma region setter

	void SetPosition(const Vector3& position)override { object_->SetPosition(position); }

	void SetScale(const Vector3& scale)override { object_->SetScale(scale); }

#pragma endregion

private:

	void OnCollision();

private:
	bool isHit_ = false;
};

