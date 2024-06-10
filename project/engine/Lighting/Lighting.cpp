#include "Lighting.h"

void Lighting::Initialize(Light light)
{
	resource_.cameraResource = CreateResource::CreateBufferResource(sizeof(Camera));
	resource_.cameraResource->Map(0, nullptr, reinterpret_cast<void**>(&cameraData_));
	cameraData_->worldPosition = { 0.0f,10.0f,-60.0f };

	if (light == Spot) {
		InitializeSpotLight();
		light_ = light;
	}
	else if (light == Point) {
		InitializePointLight();
		light_ = light;
	}
	else if (light == Environment) {
		InitializeEnvironment();
		light_ = light;
	}
}

void Lighting::InitializeSpotLight()
{
	resource_.spotLightResource = CreateResource::CreateBufferResource(sizeof(SpotLight));
	resource_.spotLightResource->Map(0, nullptr, reinterpret_cast<void**>(&spotLightData_));
	spotLightData_->color = { 1.0f,1.0f,1.0f,1.0f };
	spotLightData_->position = { 2.0f,1.25f,0.0f };
	spotLightData_->intensity = 4.0f;
	spotLightData_->direction = Normalize({ -1.0f, -1.0f, 0.0f });
	spotLightData_->distance = 7.0f;
	spotLightData_->cosAngle = std::cos(std::numbers::pi_v<float> / 3.0f);
	spotLightData_->cosFallooffStart = 1.0f;
	spotLightData_->decay = 2.0f;
}

void Lighting::InitializePointLight()
{
	resource_.pointLightResource = CreateResource::CreateBufferResource(sizeof(PointLight));
	resource_.pointLightResource->Map(0, nullptr, reinterpret_cast<void**>(&pointLightData_));
	pointLightData_->color = { 1.0f,1.0f,1.0f,1.0f };
	pointLightData_->position = { 0.0f,10.0f,0.0f };
	pointLightData_->intensity = 1.0f;
	pointLightData_->radius = 12.0f;
	pointLightData_->decay = 0.6f;
}

void Lighting::InitializeEnvironment()
{

}

void Lighting::CreateCommand()
{

	if (light_ == Point) {
		// カメラ用
		DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(5, resource_.cameraResource->GetGPUVirtualAddress());
		// ポイントライト用
		DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(6, resource_.pointLightResource->GetGPUVirtualAddress());
	}
	else if (light_ == Spot) {
		// カメラ用
		DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(5, resource_.cameraResource->GetGPUVirtualAddress());
		// ポイントライト用
		DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(6, resource_.spotLightResource->GetGPUVirtualAddress());
	}
	else if (light_ == Environment) {
		DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(5, resource_.cameraResource->GetGPUVirtualAddress());
		DirectXCommon::GetCommandList()->SetGraphicsRootDescriptorTable(6, SrvManager::GetInstance()->GetGPUHandle(index_));
	}

}
