#include "SkyBox.h"

void SkyBox::Initialize()
{
	CreateBuffer();
	CreateVertex();
}

void SkyBox::Draw(WorldTransform worldTransform, Camera& camera)
{
	pipeline_ = GraphicsPipeline::GetInstance()->GetPSO().SkyBox;

	// Rootsignatureを設定。PSOに設定してるけど別途設定が必要
	DirectXCommon::GetCommandList()->SetGraphicsRootSignature(pipeline_.rootSignature_.Get());
	DirectXCommon::GetCommandList()->SetPipelineState(pipeline_.graphicsPipelineState_.Get()); // PSOを設定

	DirectXCommon::GetCommandList()->IASetVertexBuffers(0, 1, &VBV_); // VBVを設定
	DirectXCommon::GetCommandList()->IASetIndexBuffer(&IBV_);

	// 形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけば良い
	DirectXCommon::GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// マテリアルCBufferの場所を設定
	DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(0, resource_.materialResource->GetGPUVirtualAddress());

	DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(1, worldTransform.constBuff->GetGPUVirtualAddress());
	DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(2, camera.constBuff_->GetGPUVirtualAddress());
	DirectXCommon::GetCommandList()->SetGraphicsRootDescriptorTable(3, SrvManager::GetInstance()->GetGPUHandle(texHandle_));
	// Drawコール
	DirectXCommon::GetCommandList()->DrawIndexedInstanced(36, 1, 0, 0, 0);
}

void SkyBox::CreateBuffer()
{
	resource_.vertexResource = CreateResource::CreateBufferResource(sizeof(VertexData) * 24);

	VBV_ = CreateResource::CreateVertexBufferView(resource_.vertexResource, sizeof(VertexData) * 24, 24);

	// 書き込むためのアドレスを取得
	resource_.vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));

	resource_.indexResource = CreateResource::CreateBufferResource(sizeof(uint32_t) * 36);
	IBV_ = CreateResource::CreateIndexBufferView(resource_.indexResource, sizeof(uint32_t) * 36);

	// 書き込むためのアドレスを取得
	resource_.indexResource->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));

	// material
	resource_.materialResource = CreateResource::CreateBufferResource(sizeof(Material));
	resource_.materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	materialData_->color = { 1.0f,1.0f,1.0f,1.0f };
	materialData_->enableLighting = false;
	//materialData_->shininess = 70.0f;
}

void SkyBox::CreateVertex()
{
#pragma region 頂点データ
	// 右面
	vertexData_[0].position = { 1.0f,1.0f,1.0f,1.0f };
	vertexData_[1].position = { 1.0f,1.0f,-1.0f,1.0f };
	vertexData_[2].position = { 1.0f,-1.0f,1.0f,1.0f };
	vertexData_[3].position = { 1.0f,-1.0f,-1.0f,1.0f };
    // 左面
	vertexData_[4].position = { -1.0f,1.0f,-1.0f,1.0f };
	vertexData_[5].position = { -1.0f,1.0f,1.0f,1.0f };
	vertexData_[6].position = { -1.0f,-1.0f,-1.0f,1.0f };
	vertexData_[7].position = { -1.0f,-1.0f,1.0f,1.0f };
	// 前面
	vertexData_[8].position = { -1.0f,1.0f,1.0f,1.0f };
	vertexData_[9].position = { 1.0f,1.0f,1.0f,1.0f };
	vertexData_[10].position = { -1.0f,-1.0f,1.0f,1.0f };
	vertexData_[11].position = { 1.0f,-1.0f,1.0f,1.0f };
	// 後面
	vertexData_[12].position = { 1.0f,1.0f,-1.0f,1.0f };
	vertexData_[13].position = { -1.0f,1.0f,-1.0f,1.0f };
	vertexData_[14].position = { 1.0f,-1.0f,-1.0f,1.0f };
	vertexData_[15].position = { -1.0f,-1.0f,-1.0f,1.0f };
	// 上面
	vertexData_[16].position = { -1.0f,1.0f,-1.0f,1.0f };
	vertexData_[17].position = { 1.0f,1.0f,-1.0f,1.0f };
	vertexData_[18].position = { -1.0f,1.0f,1.0f,1.0f };
	vertexData_[19].position = { 1.0f,1.0f,1.0f,1.0f };
	// 下面
	vertexData_[20].position = { 1.0f,-1.0f,-1.0f,1.0f };
	vertexData_[21].position = { -1.0f,-1.0f,-1.0f,1.0f };
	vertexData_[22].position = { 1.0f,-1.0f,1.0f,1.0f };
	vertexData_[23].position = { -1.0f,-1.0f,1.0f,1.0f };

#pragma endregion

#pragma region インデックスデータ

	// 右面
	indexData_[0] = 0; indexData_[1] = 1; indexData_[2] = 2;
	indexData_[3] = 2; indexData_[4] = 1; indexData_[5] = 3;
	// 左面
	indexData_[6] = 4; indexData_[7] = 5; indexData_[8] = 6;
	indexData_[9] = 6; indexData_[10] = 5; indexData_[11] = 7;
	// 前面
	indexData_[12] = 8; indexData_[13] = 9; indexData_[14] = 10;
	indexData_[15] = 10; indexData_[16] = 9; indexData_[17] = 11;
	// 後面
	indexData_[18] = 12; indexData_[19] = 13; indexData_[20] = 14;
	indexData_[21] = 14; indexData_[22] = 13; indexData_[23] = 15;
	// 上面
	indexData_[24] = 16; indexData_[25] = 17; indexData_[26] = 18;
	indexData_[27] = 18; indexData_[28] = 17; indexData_[29] = 19;
	// 下面
	indexData_[30] = 20; indexData_[31] = 21; indexData_[32] = 22;
	indexData_[33] = 22; indexData_[34] = 21; indexData_[35] = 23;

#pragma endregion

}				 
