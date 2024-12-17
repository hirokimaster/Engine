/**
* @file Object3DPlacer.h
* @brief モデルの位置決め、配置を行うクラス
* @author 仁平 琉乃
*/

#include "Object3DPlacer.h"

Object3DPlacer::Object3DPlacer()
{
}

Object3DPlacer::~Object3DPlacer()
{
	// デストラクタが呼ばれたらinstancing用のSRVがあるとこ解放する
	SrvManager::GetInstance()->StructuredBufIndexFree(srvIndex_);
}

void Object3DPlacer::Initialize(bool isInstancing, uint32_t numInstance)
{
	isInstancing_ = isInstancing;
	numInstance_ = numInstance;
	if (!isInstancing_) {
		worldTransform_.Initialize();
		resource_.materialResource = CreateResource::CreateBufferResource(sizeof(Material));
		resource_.materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
		materialData_->color = { 1.0f,1.0f,1.0f,1.0f };
		materialData_->enableLighting = true;
		materialData_->shininess = 20.0f;
		materialData_->uvTransform = MakeIdentityMatrix();
		materialData_->environmentCoefficient = 1.0f;

		// 平行光源用のリソース
		resource_.directionalLightResource = CreateResource::CreateBufferResource(sizeof(DirectionalLight));
		// 書き込むためのアドレスを取得
		resource_.directionalLightResource->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData_));
		directionalLightData_->color = { 1.0f,1.0f,1.0f,1.0f };
		directionalLightData_->direction = Normalize({ 20.0f, -20.0f, 0.0f });
		directionalLightData_->intensity = 1.0f;
	}
	else {
		InitializeInstancing();
	}
	
}

void Object3DPlacer::Draw(Camera& camera)
{
	CreateUVTransformMatrix();

	CreatePipeline();

	// Rootsignatureを設定。PSOに設定してるけど別途設定が必要
	DirectXCommon::GetCommandList()->SetGraphicsRootSignature(pipelineData_.rootSignature_.Get());
	DirectXCommon::GetCommandList()->SetPipelineState(pipelineData_.graphicsPipelineState_.Get()); // PSOを設定

	// 形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけば良い
	DirectXCommon::GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// マテリアルCBufferの場所を設定
	DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(0, resource_.materialResource->GetGPUVirtualAddress());

	if (!isInstancing_) {
		// wvp用のCBufferの場所を設定
		DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(1, worldTransform_.constBuff->GetGPUVirtualAddress());
	}
	else {
		DirectXCommon::GetCommandList()->SetGraphicsRootDescriptorTable(1, SrvManager::GetInstance()->GetGPUHandle(srvIndex_));
	}
	
	DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(2, camera.constBuff_->GetGPUVirtualAddress());
	DirectXCommon::GetCommandList()->SetGraphicsRootDescriptorTable(3, SrvManager::GetInstance()->GetGPUHandle(texHandle_));
	// 平行光源
	DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(4, resource_.directionalLightResource->GetGPUVirtualAddress());

	if (lighting_) {
		lighting_->CreateCommand();
	}

	if (model_ && !isInstancing_) {
		model_->Draw();
	}
	else if (model_ && isInstancing_) {
		model_->Draw(numInstance_);
	}
}

void Object3DPlacer::Draw(Camera& camera, bool isAnimation)
{
	worldTransform_.UpdateMatrix();

	pipelineData_ = GraphicsPipeline::GetInstance()->GetPSO().SkinningObject3D;

	// Rootsignatureを設定。PSOに設定してるけど別途設定が必要
	DirectXCommon::GetCommandList()->SetGraphicsRootSignature(pipelineData_.rootSignature_.Get());
	DirectXCommon::GetCommandList()->SetPipelineState(pipelineData_.graphicsPipelineState_.Get()); // PSOを設定

	// 形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけば良い
	DirectXCommon::GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// マテリアルCBufferの場所を設定
	DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(0, resource_.materialResource->GetGPUVirtualAddress());
	DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(1, worldTransform_.constBuff->GetGPUVirtualAddress());
	DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(2, camera.constBuff_->GetGPUVirtualAddress());
	DirectXCommon::GetCommandList()->SetGraphicsRootDescriptorTable(4, SrvManager::GetInstance()->GetGPUHandle(texHandle_));
	// 平行光源
	DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(5, resource_.directionalLightResource->GetGPUVirtualAddress());

	if (modelAnimation_) {
		modelAnimation_->Draw(isAnimation);
	}
}

void Object3DPlacer::InitializeInstancing()
{
	// Instancing用のResourceを作る
	resource_.instancingResource = CreateResource::CreateBufferResource(sizeof(ConstBufferDataWorldTransform) * numInstance_);
	// 書き込むためのアドレスを取得
	resource_.instancingResource->Map(0, nullptr, reinterpret_cast<void**>(&instancingData_));
	// インスタンス数
	const uint32_t instance = numInstance_;
	// worldTransformの初期化
	for (uint32_t i = 0; i < instance; ++i) {
		worldTransformInstancing_[i].Initialize();
	}
	// 単位行列を書き込む
	for (uint32_t index = 0; index < instance; ++index) {
		instancingData_[index].matWorld = MakeIdentityMatrix();
		instancingData_[index].world = MakeIdentityMatrix();
		instancingData_[index].WorldInverseTranspose = MakeIdentityMatrix();
	}

	// srvの位置をtextureのsrvの位置から設定する
	SrvManager::GetInstance()->StructuredBufIndexAllocate();
	srvIndex_ = SrvManager::GetInstance()->GetStructuredBufIndex();
	SrvManager::GetInstance()->CreateInstancingSrv(resource_.instancingResource, srvIndex_);

	resource_.materialResource = CreateResource::CreateBufferResource(sizeof(Material));
	resource_.materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	materialData_->color = { 1.0f,1.0f,1.0f,1.0f };
	materialData_->enableLighting = true;
	materialData_->shininess = 20.0f;
	materialData_->uvTransform = MakeIdentityMatrix();
	materialData_->environmentCoefficient = 1.0f;

	// 平行光源用のリソース
	resource_.directionalLightResource = CreateResource::CreateBufferResource(sizeof(DirectionalLight));
	// 書き込むためのアドレスを取得
	resource_.directionalLightResource->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData_));
	directionalLightData_->color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLightData_->direction = Normalize({ 20.0f, -20.0f, 0.0f });
	directionalLightData_->intensity = 1.0f;
}

void Object3DPlacer::UpdateInstancing()
{
	// インスタンス数
	const uint32_t instance = numInstance_;
	// 各インスタンスの行列を計算して定数バッファに書き込む
	for (uint32_t i = 0; i < instance; ++i) {
		worldTransformInstancing_[i].UpdateMatrix(); // 行列を更新
		instancingData_[i].matWorld = worldTransformInstancing_[i].matWorld;
		instancingData_[i].world = worldTransformInstancing_[i].world;
		instancingData_[i].WorldInverseTranspose = InverseTranspose(worldTransformInstancing_[i].matWorld);
	}
}

void Object3DPlacer::CreatePipeline()
{
	if (!isInstancing_) {
		if (lighting_ == nullptr) {
			pipelineData_ = GraphicsPipeline::GetInstance()->GetPSO().Object3D;
		}
		else {
			if (lighting_->GetLightType() == Light::None) {
				pipelineData_ = GraphicsPipeline::GetInstance()->GetPSO().Object3D;
			}
			else if (lighting_->GetLightType() == Light::Point) {
				pipelineData_ = GraphicsPipeline::GetInstance()->GetPSO().PointLight;
			}
			else if (lighting_->GetLightType() == Light::Spot) {
				pipelineData_ = GraphicsPipeline::GetInstance()->GetPSO().SpotLight;
			}
			else if (lighting_->GetLightType() == Light::Environment) {
				pipelineData_ = GraphicsPipeline::GetInstance()->GetPSO().Environment;
			}
		}
	}
	else {
		pipelineData_ = GraphicsPipeline::GetInstance()->GetPSO().Object3DInstancing;
	}
}

void Object3DPlacer::CreateUVTransformMatrix()
{
	Matrix4x4 uvTransformMatrix = MakeScaleMatrix(uvTransform_.scale);
	uvTransformMatrix = Multiply(uvTransformMatrix, MakeRotateZMatrix(uvTransform_.rotate.z));
	uvTransformMatrix = Multiply(uvTransformMatrix, MakeTranslateMatrix(uvTransform_.translate));
	materialData_->uvTransform = uvTransformMatrix;
}
