#include "Line.h"

void Line::Initialize(Primitive* state, uint32_t texHandle)
{
	// buffer作成
	CreateBuffer();

}

void Line::Draw(WorldTransform& worldTransform, Camera& camera)
{
	// 頂点データ作成
	CreateVertex();

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
	*materialData = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
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
