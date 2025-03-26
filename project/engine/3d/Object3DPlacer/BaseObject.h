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

protected:
	std::unique_ptr<Object3DPlacer> object_;
	std::unique_ptr<Collider> collider_;
};
