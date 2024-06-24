#pragma once
#include "engine/Model/Primitive/IPrimitive.h"
#include "engine/GraphicsPipeline/GraphicsPipeline.h"

class Line : public	IPrimitive {
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="state"></param>
	/// <param name="texHandle"></param>
	void Initialize(Primitive* state)override;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="worldTransform"></param>
	/// <param name="camera"></param>
	void Draw(WorldTransform worldTransform, Camera& camera)override;

#pragma region setter

	/// <summary>
    /// 最初の位置と最後の位置
    /// </summary>
    /// <param name="pos"></param>
	void SetStartPos(Vector3 pos) { start_ = pos; }
	void SetEndPos(Vector3 pos) { end_ = pos; }

	/// <summary>
	/// 色
	/// </summary>
	/// <param name="color"></param>
	void SetColor(Vector4 color) { color_ = color; }

#pragma endregion

private:

	/// <summary>
	/// buffer作成
	/// </summary>
	void CreateBuffer();

	/// <summary>
	/// 頂点データ作成
	/// </summary>
	void CreateVertex();

private:

	Vector3 start_{0,0,0};
	Vector3 end_{10.0f,0.0f,0.0f};
	D3D12_VERTEX_BUFFER_VIEW VBV{};
	Resource resource_ = {};
	Vector4 color_{ 1.0f,1.0f,1.0f,1.0f };
};
