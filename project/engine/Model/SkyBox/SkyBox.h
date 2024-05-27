#pragma once
#include "engine/GraphicsPipeline/GraphicsPipeline.h"
#include "engine/DescriptorManager/SRVManager/SrvManager.h"
#include "engine/Transform/WorldTransform.h"

class SkyBox{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="worldTransform"></param>
	/// <param name="camera"></param>
	void Draw(WorldTransform worldTransform, Camera& camera);

	/// <summary>
	/// textureのsrvHandleを設定
	/// </summary>
	/// <param name="texHandle"></param>
	void SetTexHandle(uint32_t texHandle) { texHandle_ = texHandle; }

private:

	/// <summary>
	/// resouce作成
	/// </summary>
	void CreateBuffer();

	/// <summary>
	/// 頂点情報作成
	/// </summary>
	void CreateVertex();

private:
	Property pipeline_{};
	D3D12_INDEX_BUFFER_VIEW IBV_{};
	D3D12_VERTEX_BUFFER_VIEW VBV_{};
	Resource resource_ = {};
	VertexData* vertexData_ = nullptr;
	uint32_t* indexData_ = nullptr;
	Material* materialData_ = nullptr;
	uint32_t texHandle_ = 0;
};
