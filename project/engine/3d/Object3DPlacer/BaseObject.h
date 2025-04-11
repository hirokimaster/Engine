#pragma once
#include "engine/3d/Object3DPlacer/Object3DPlacer.h"
#include "engine/Utility/CollisionManager/Collider/Collider.h"
#include "engine/Graphics/TextureManager/TextureManager.h"

class BaseObject {
public:
	/// <summary>
	/// 共通処理初期化
	/// </summary>
	/// <param name="modelName"></param>
	/// <param name="textureName"></param>
	/// <param name="type"></param>
	virtual void Initialize(const std::string& modelName, const std::string& textureName, ColliderType type = ColliderType::None);

	/// <summary>
	/// 共通処理更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 共通処理描画
	/// </summary>
	/// <param name="camera"></param>
	virtual void Draw(const Camera& camera);

#pragma region getter

	virtual Collider* GetCollider() { 
		if (collider_) {
			return collider_.get();
		}
		return nullptr;
	}

#pragma region setter

	virtual void SetPosition(const Vector3& position) { object_->SetPosition(position); }
	virtual void SetRotate(const Vector3& rotate) { object_->SetRotate(rotate); }
	virtual void SetScale(const Vector3& scale) { object_->SetScale(scale); }
	virtual void SetUVTransform(const UVTransform& uv) { object_->SetUVTransform(uv); }

#pragma endregion

protected:
	std::unique_ptr<Object3DPlacer> object_;
	std::unique_ptr<Collider> collider_;
};
