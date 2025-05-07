/**
* @file BaseIndividualObject.h
* @brief  単一オブジェクトの基底クラス
* @author 仁平 琉乃
*/

#pragma once
#include "engine/Utility/CollisionManager/Collider/Collider.h"
#include "engine/Graphics/TextureManager/TextureManager.h"
#include "engine/3d/ObjectManager/ObjectManager.h"

class BaseIndividualObject {
public:
	/// <summary>
	/// 仮想デストラクタ
	/// </summary>
	virtual ~BaseIndividualObject();

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

#pragma region getter

	Collider* GetCollider() {
		return collider_ ? collider_.get() : nullptr;
	}

#pragma endregion

#pragma region setter

	void SetPosition(const Vector3& position) {
		if (auto obj = object_.lock()) {
			obj->SetPosition(position);
		}
	}
	
	void SetRotate(const Vector3& rotate) {
		if (auto obj = object_.lock()) {
			obj->SetRotate(rotate);
		}
	}

	void SetScale(const Vector3& scale) {
		if (auto obj = object_.lock()) {
			obj->SetScale(scale);
		}
	}

#pragma endregion

protected:
	std::weak_ptr<Object3dPlacer> object_;
	std::unique_ptr<Collider> collider_;
	ObjectManager* objectManager_ = nullptr;
};