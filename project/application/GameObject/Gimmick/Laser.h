#pragma once
#include "engine/3d/Object3DPlacer/Object3DPlacer.h"
#include "engine/Utility/CollisionManager/Collider/Collider.h"
#include "engine/Graphics/TextureManager/TextureManager.h"

class Laser{
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
	void Draw(Camera& camera);

#pragma region getter

	Vector3 GetWorldPosition() const;

	Vector3 GetScale() const{ return object_->GetWorldTransform().scale; }

	bool GetIsHit()const { return isHit_; }

	Collider* GetCollider() { return collider_.get(); }

#pragma endregion

#pragma region setter

	void SetPosition(Vector3 position) { object_->SetPosition(position); }

#pragma endregion

private:

	void OnCollision();

private:
	bool isHit_ = false;
	std::unique_ptr<Collider> collider_ = nullptr;
	std::unique_ptr<Object3DPlacer> object_ = nullptr;
};
