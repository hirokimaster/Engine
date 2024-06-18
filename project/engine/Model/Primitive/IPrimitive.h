#pragma once
#include "engine/Transform/WorldTransform.h"

class Primitive;

class IPrimitive {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="state"></param>
	virtual void Initialize(Primitive* state) = 0;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="worldTransform"></param>
	/// <param name="camera"></param>
	virtual void Draw(WorldTransform worldTransform, Camera& camera) = 0;

protected:

};