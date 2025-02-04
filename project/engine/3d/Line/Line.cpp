#include "Line.h"



Line::Line()
{
}

Line::~Line()
{
}

void Line::Initialize(const Vector3& start, const Vector3& end)
{
	// ワールド行列初期化
	worldTransform_.Initialize();
	// 線の始点と終点
	startPosition_ = start;
	endPosition_ = end;
	// buffer作成
	CreateBuffer();
	// 頂点データ作成
	CreateVertex();
}

void Line::Draw(const Camera& camera)
{
	// 行列の更新
	worldTransform_.UpdateMatrix();
	// pipelineState取得
	pipelineData_ = GraphicsPipeline::GetInstance()->GetPSO().Line;

	// rootSignatureを設定
	DirectXCommon::GetCommandList()->SetGraphicsRootSignature(pipelineData_.rootSignature_.Get());
	DirectXCommon::GetCommandList()->SetPipelineState(pipelineData_.graphicsPipelineState_.Get()); // PSOを設定
	DirectXCommon::GetCommandList()->IASetVertexBuffers(0, 1, &VBV_); // VBVを設定
	// 形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけば良い
	DirectXCommon::GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	// マテリアルCBufferの場所を設定
	DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(1, worldTransform_.constBuff->GetGPUVirtualAddress());
	DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(2, camera.constBuff_->GetGPUVirtualAddress());
	// 描画。(DrawCall/ドローコール)。2頂点で1つのインスタンス
	DirectXCommon::GetCommandList()->DrawInstanced(2, 1, 0, 0);

}

void Line::CreateVertex()
{
	// 頂点リソースにデータを書き込む
	VertexData* vertexData = nullptr;
	// 書き込むためのアドレスを取得
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));

	vertexData[0].position = { startPosition_.x,startPosition_.y, startPosition_.z,1.0f };
	vertexData[1].position = { endPosition_.x,endPosition_.y,endPosition_.z,1.0f };
}

void Line::CreateBuffer()
{
	// 頂点バッファ作成
	vertexResource_ = CreateResource::CreateBufferResource(sizeof(VertexData) * 2);
	VBV_ = CreateResource::CreateVertexBufferView(vertexResource_.Get(), sizeof(VertexData) * 2, 2);

	// マテリアルのバッファ作成
	materialResource_ = CreateResource::CreateBufferResource(sizeof(Vector4));
	// データ書き込み
	Vector4* materialData = nullptr;
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	*materialData = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
}
