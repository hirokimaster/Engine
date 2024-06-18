#include "Line.h"

void Line::Initialize(Primitive* state)
{
	// buffer作成
	CreateBuffer();
	state;
}

void Line::Draw(WorldTransform worldTransform, Camera& camera)
{
	// 頂点データ作成
	CreateVertex();
	// graphicsPipeline
	Property pipeline = GraphicsPipeline::GetInstance()->GetPSO().Line;
	// Rootsignatureを設定。PSOに設定してるけど別途設定が必要
	DirectXCommon::GetCommandList()->SetGraphicsRootSignature(pipeline.rootSignature_.Get());
	DirectXCommon::GetCommandList()->SetPipelineState(pipeline.graphicsPipelineState_.Get()); // PSOを設定
	// マテリアルCBufferの場所を設定
	DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(0, resource_.materialResource->GetGPUVirtualAddress());
	// transformationMatrix
	DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(1, worldTransform.constBuff->GetGPUVirtualAddress());
	// camera
	DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(2, camera.constBuff_->GetGPUVirtualAddress());
	// 描画。(DrawCall/ドローコール)。2頂点で1つのインスタンス
	DirectXCommon::GetCommandList()->DrawInstanced(2, 1, 0, 0);
}

void Line::CreateBuffer()
{
	resource_.vertexResource = CreateResource::CreateBufferResource(sizeof(VertexData) * 2);

	VBV = CreateResource::CreateVertexBufferView(resource_.vertexResource, sizeof(VertexData) * 2, 2);

	resource_.materialResource = CreateResource::CreateBufferResource(sizeof(Vector4));
	// データを書き込む
	Vector4* materialData = nullptr;
	// アドレスを取得
	resource_.materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	// 白
	*materialData = Vector4(color_);
}

void Line::CreateVertex()
{

	// 頂点リソースにデータを書き込む
	VertexData* vertexData = nullptr;
	// 書き込むためのアドレスを取得
	resource_.vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));

	vertexData[0].position = { start_.x,start_.y, start_.z,1.0f };
	vertexData[1].position = { end_.x,end_.y,end_.z,1.0f };

}
