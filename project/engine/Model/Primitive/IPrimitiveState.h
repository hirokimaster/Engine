#pragma once
#include "engine/Transform/WorldTransform.h"

class Primitive;

class IPrimitiveState {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="state"></param>
	virtual void Initialize(Primitive* state, uint32_t texHandle) = 0;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="worldTransform"></param>
	/// <param name="camera"></param>
	virtual void Draw(WorldTransform& worldTransform, Camera& camera) = 0;

};