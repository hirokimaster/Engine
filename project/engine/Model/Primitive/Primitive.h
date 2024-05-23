#pragma once
#include "engine/Model/Primitive/IPrimitiveState.h"

class Primitive {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(IPrimitiveState* state);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(WorldTransform& worldTransform, Camera& camera);

	/// <summary>
	/// textureのsrvHandleを設定
	/// </summary>
	/// <param name="texHandle"></param>
	void SetTexHandle(uint32_t texHandle) { texHandle_ = texHandle; }

private:
	// 描画する図形のタイプ
	IPrimitiveState* state_ = nullptr;
	uint32_t texHandle_ = 0;
};
