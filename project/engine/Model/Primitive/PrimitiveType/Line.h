#pragma once
#include "engine/Model/Primitive/IPrimitiveState.h"

class Line : public	IPrimitiveState {
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="state"></param>
	/// <param name="texHandle"></param>
	void Initialize(Primitive* state, uint32_t texHandle)override;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="worldTransform"></param>
	/// <param name="camera"></param>
	void Draw(WorldTransform& worldTransform, Camera& camera)override;

private:

	/// <summary>
	/// buffer作成
	/// </summary>
	void CreateBuffer();

	/// <summary>
	/// 頂点データ作成
	/// </summary>
	void CreateVertex();

	/// <summary>
	/// 最初の位置と最後の位置
	/// </summary>
	/// <param name="pos"></param>
	void SetStartPos(Vector3 pos) { start_ = pos; }
	void SetEndPos(Vector3 pos) { end_ = pos; }

private:

	Vector3 start_{};
	Vector3 end_{};
	D3D12_VERTEX_BUFFER_VIEW VBV{};
	Resource resource_ = {};
};
