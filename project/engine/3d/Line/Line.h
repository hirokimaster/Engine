#pragma once
#include "engine/Math/Vector3.h"
#include "engine/Graphics/CreateResource/CreateResource.h"
#include "engine/Graphics/Camera/Camera.h"
#include "engine/Graphics/Transform/WorldTransform.h"
#include "engine/Graphics/Pipeline/GraphicsPipeline.h"

class Line {
public:

	Line();
	~Line();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="start"></param>
	/// <param name="end"></param>
	void Initialize(const Vector3& start, const Vector3& end);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	void Draw(const Camera& camera);

#pragma region setter

	void SetStartPosition(const Vector3& position) { startPosition_ = position; }
	void SetEndPosition(const Vector3& position) { endPosition_ = position; }
	void SetPosition(const Vector3& position) { worldTransform_.translate = position; }

#pragma endregion

private:
	/// <summary>
	/// 頂点データ作成
	/// </summary>
	void CreateVertex();

	/// <summary>
	/// buffer作成
	/// </summary>
	void CreateBuffer();

private:

	Vector3 startPosition_{};
	Vector3 endPosition_{};
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_;
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_;
	D3D12_VERTEX_BUFFER_VIEW VBV_{};
	WorldTransform worldTransform_{};
	GraphicsPipelineData pipelineData_{};

};
