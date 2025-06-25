/**
* @file BaseInstancingObject.h
* @brief インスタンシングオブジェクトの基底クラス
* @author 仁平 琉乃
*/

#pragma once
#include "engine/Utility/CollisionManager/Collider/Collider.h"
#include "engine/Graphics/TextureManager/TextureManager.h"
#include "engine/3d/ObjectManager/ObjectManager.h"

class BaseInstancingObject {
public:
	/// <summary>
	/// 仮想デストラクタ
	/// </summary>
	virtual ~BaseInstancingObject();

	/// <summary>
	/// 共通処理初期化
	/// </summary>
	/// <param name="modelName"></param>
	/// <param name="textureName"></param>
	/// <param name="type"></param>
	virtual void Initialize(const std::string& modelName, const std::string& textureName, ColliderType type = ColliderType::None, bool shadow = false);

	/// <summary>
	/// 共通処理更新
	/// </summary>
	virtual void Update();

#pragma region getter

	Collider* GetCollider() {
		return collider_ ? collider_.get() : nullptr;
	}

#pragma endregion

#pragma region setter

	void SetPosition(const Vector3& position) {
		if (auto obj = object_.lock()) {
			obj->worldTransform.translate = position;
		}
	}

	void SetRotate(const Vector3& rotate) {
		if (auto obj = object_.lock()) {
			obj->worldTransform.rotate = rotate;
		}
	}

	void SetScale(const Vector3& scale) {
		if (auto obj = object_.lock()) {
			obj->worldTransform.scale = scale;
		}
	}

	void SetTexHandle(uint32_t handle) { object_.lock()->texHandel = handle; }

#pragma endregion

protected:
	std::weak_ptr<Object3dInstancing> object_;
	std::unique_ptr<Collider> collider_;
	ObjectManager* objectManager_ = nullptr;
};
