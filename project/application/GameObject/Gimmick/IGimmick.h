#pragma once
#include "engine/Utility/CollisionManager/CollisionManager.h"
#include "engine/Object3DPlacer/Object3DPlacer.h"

class IGimmick : public Collider{
public:
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
	virtual void Draw(const Camera& camera) = 0;

protected:
	WorldTransform worldTransform_{};
	std::unique_ptr<Object3DPlacer> object_ = nullptr;
};
