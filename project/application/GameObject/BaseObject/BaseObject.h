#pragma once
#include "engine/Utility/CollisionManager/CollisionManager.h"
#include "engine/Object3DPlacer/Object3DPlacer.h"
#include "engine/TextureManager/TextureManager.h"

class BaseObject : public Collider{
public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~BaseObject();

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw(Camera& camera);

	/// <summary>
	/// 当たり判定
	/// </summary>
	virtual void OnCollision()override = 0;

#pragma region getter

	virtual Vector3 GetWorldPosition() const override = 0;

	virtual Vector3 GetScale()const override = 0;

#pragma endregion

#pragma region setter

	virtual void SetPosition(Vector3 position) = 0;

#pragma endregion

protected:
	std::unique_ptr<Object3DPlacer> object_ = nullptr;
};
