/**
* @file Object3DPlacer.h
* @brief モデルの配置を行うクラス
* @author 仁平 琉乃
*/

#include "Object3DPlacer.h"
#include <engine/Graphics/TextureManager/TextureManager.h>

void Object3DPlacer::Initialize()
{
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
	directionalLightData_->direction = Normalize({ 0.0f, -10.0f, 0.0f });
	directionalLightData_->intensity = 1.0f;
}

void Object3DPlacer::Update()
{
	worldTransform_.UpdateMatrix();
}

void Object3DPlacer::Draw(const Camera& camera)
{
	CreateUVTransformMatrix();

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
	
	// Rootsignatureを設定。PSOに設定してるけど別途設定が必要
	DirectXCommon::GetCommandList()->SetGraphicsRootSignature(pipelineData_.rootSignature_.Get());
	DirectXCommon::GetCommandList()->SetPipelineState(pipelineData_.graphicsPipelineState_.Get()); // PSOを設定

	// 形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけば良い
	DirectXCommon::GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// マテリアルCBufferの場所を設定
	DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(0, resource_.materialResource->GetGPUVirtualAddress());

	// wvp用のCBufferの場所を設定
	DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(1, worldTransform_.constBuff->GetGPUVirtualAddress());
	DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(2, camera.constBuff_->GetGPUVirtualAddress());
	DirectXCommon::GetCommandList()->SetGraphicsRootDescriptorTable(3, SrvManager::GetInstance()->GetGPUHandle(texHandle_));
	// 平行光源
	DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(4, resource_.directionalLightResource->GetGPUVirtualAddress());

	if (lighting_) {
		lighting_->CreateCommand();
	}

	if (model_) {
		model_->Draw();
	}
}

void Object3DPlacer::Draw(const Camera& camera, bool isAnimation)
{
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

void Object3DPlacer::CreateUVTransformMatrix()
{
	Matrix4x4 uvTransformMatrix = MakeScaleMatrix(uvTransform_.scale);
	uvTransformMatrix = Multiply(uvTransformMatrix, MakeRotateZMatrix(uvTransform_.rotate.z));
	uvTransformMatrix = Multiply(uvTransformMatrix, MakeTranslateMatrix(uvTransform_.translate));
	materialData_->uvTransform = uvTransformMatrix;
}
