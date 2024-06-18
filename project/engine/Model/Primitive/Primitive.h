#pragma once
#include "engine/Model/Primitive/IPrimitive.h"

class Primitive {
public:

	Primitive();
	~Primitive();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(IPrimitive* state);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(WorldTransform worldTransform, Camera& camera);

	/// <summary>
	/// textureのsrvHandleを設定
	/// </summary>
	/// <param name="texHandle"></param>
	void SetTexHandle(uint32_t texHandle) { texHandle_ = texHandle; }

private:
	// 描画する図形のタイプ
	IPrimitive* state_ = nullptr;
	uint32_t texHandle_ = 0;
};
