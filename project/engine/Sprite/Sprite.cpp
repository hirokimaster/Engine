#include "Sprite.h"
#include "engine/Math/Vector4.h"

Camera Sprite::camera_;

Sprite::~Sprite()
{
	
}

void Sprite::StaticInitialize()
{
	camera_.Initialize();
}

void Sprite::StaticUpdate()
{
	camera_.UpdateMatrix();
}

/// <summary>
/// 初期化
/// </summary>
void Sprite::Initialize(uint32_t texHandle) {

	worldTransform_.Initialize();

	texHandle_ = texHandle;

	// indexResourceSprite
	resource_.indexResource = CreateResource::CreateBufferResource(sizeof(uint32_t) * 6);
	// バッファービューを作成
	IBV_ = CreateResource::CreateIndexBufferView(resource_.indexResource, sizeof(uint32_t) * 6);
	// インデックスリソースにデータを書き込む
	uint32_t* indexDataSprite = nullptr;
	resource_.indexResource->Map(0, nullptr, reinterpret_cast<void**>(&indexDataSprite));
	indexDataSprite[0] = 0;
	indexDataSprite[1] = 1;
	indexDataSprite[2] = 2;
	indexDataSprite[3] = 1;
	indexDataSprite[4] = 3;
	indexDataSprite[5] = 2;


	resource_.materialResource = CreateResource::CreateBufferResource(sizeof(Vector4));
	// データを書き込む
	// アドレスを取得
	resource_.materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	// 赤
	*materialData_ = Vector4(1.0f,1.0f,1.0f,1.0f);

	resource_.vertexResource = CreateResource::CreateBufferResource(sizeof(VertexData) * 4);

	VBV_ = CreateResource::CreateVertexBufferView(resource_.vertexResource, sizeof(VertexData) * 4, 4);

	// 頂点データの設定
    vertexDataSprite_ = nullptr;
	resource_.vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexDataSprite_));

	AdjustTextureSize(texHandle_);

	textureSize_ = size_;

	worldTransform_.Initialize();

}

/// <summary>
/// 頂点データ作成
/// </summary>
void Sprite::CreateVertex()
{

	float left = (0.0f - anchorPoint_.x) * size_.x;
	float right = (1.0f - anchorPoint_.x) * size_.x;
	float top = (0.0f - anchorPoint_.y) * size_.y;
	float bottom = (1.0f - anchorPoint_.y) * size_.y;

	float tex_left = textureLeftTop_.x / size_.x;
	float tex_right = (textureLeftTop_.x + textureSize_.x) / size_.x;
	float tex_top = textureLeftTop_.y / size_.y;
	float tex_bottom = (textureLeftTop_.y + textureSize_.y) / size_.y;


	// 1枚目の三角形
	vertexDataSprite_[0].position = { left, bottom,0.0f, 1.0f }; // 左下
	vertexDataSprite_[0].texcoord = { tex_left, tex_bottom };
	vertexDataSprite_[1].position = { left, top, 0.0f, 1.0f }; // 左上
	vertexDataSprite_[1].texcoord = { tex_left, tex_top };
	vertexDataSprite_[2].position = { right, bottom, 0.0f,1.0f }; // 右下
	vertexDataSprite_[2].texcoord = { tex_right, tex_bottom };
	// 2枚目の三角形
	vertexDataSprite_[3].position = { right, top, 0.0f, 1.0f }; // 右上
	vertexDataSprite_[3].texcoord = { tex_right, tex_top };

}


/// <summary>
/// スプライト生成
/// </summary>
/// <param name="position"></param>
/// <returns></returns>
Sprite* Sprite::Create(uint32_t texHandle, Vector2 position, Vector4 color)
{
	Sprite* sprite = new Sprite;
	sprite->Initialize(texHandle);
    sprite->SetPosition(position);
	sprite->SetColor(color);

	return sprite;
}

void Sprite::AdjustTextureSize(uint32_t texHandle)
{
	const DirectX::TexMetadata& metadata = TextureManager::GetInstance()->GetMetaData(texHandle);

	size_.x = static_cast<float>(metadata.width);
	size_.y = static_cast<float>(metadata.height);

}

/// <summary>
/// 描画
/// </summary>
/// <param name="v"></param>
/// <param name="t"></param>
void Sprite::Draw()
{
	
	CreateVertex();
	worldTransform_.translate.x = GetPosition().x;
	worldTransform_.translate.y = GetPosition().y;
	worldTransform_.UpdateMatrix();

	Property property = GraphicsPipeline::GetInstance()->GetPSO().Sprite2D;

	// Rootsignatureを設定。PSOに設定してるけど別途設定が必要
	DirectXCommon::GetCommandList()->SetGraphicsRootSignature(property.rootSignature_.Get());
	DirectXCommon::GetCommandList()->SetPipelineState(property.graphicsPipelineState_.Get()); // PSOを設定
	DirectXCommon::GetCommandList()->IASetVertexBuffers(0, 1, &VBV_); // VBVを設定	
	DirectXCommon::GetCommandList()->IASetIndexBuffer(&IBV_); // IBVの設定
	// 形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけば良い
	DirectXCommon::GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// マテリアルCBufferの場所を設定
	DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(0, resource_.materialResource->GetGPUVirtualAddress());
	// wvp用のCBufferの場所を設定
	DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(1, worldTransform_.constBuff->GetGPUVirtualAddress());
	DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(2, camera_.sConstBuff->GetGPUVirtualAddress());
	DirectXCommon::GetCommandList()->SetGraphicsRootDescriptorTable(3, SrvManager::GetInstance()->GetGPUHandle(texHandle_));
	// 描画。(DrawCall/ドローコール)。3頂点で1つのインスタンス。インスタンスについては今後
	DirectXCommon::GetCommandList()->DrawIndexedInstanced(6, 1, 0, 0, 0);

}
