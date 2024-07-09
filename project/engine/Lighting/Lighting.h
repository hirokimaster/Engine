#pragma once
#include "engine/Model/Model.h"
#include "engine/DescriptorManager/SRVManager/SrvManager.h"

class Lighting {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Light light);

	/// <summary>
	///  描画
	/// </summary>
	/// <param name="light"></param>
	void CreateCommand();

private:

	/// <summary>
    /// spotLight
    /// </summary>
	void InitializeSpotLight();

	/// <summary>
	/// pointLight
	/// </summary>
	void InitializePointLight();

	/// <summary>
	/// 環境マップ
	/// </summary>
	void InitializeEnvironment();

public:

#pragma region	getter

	Light GetLightType() { return light_; }

#pragma endregion

#pragma region	setter

	// lightの設定
	PointLight SetPointLightProperty(PointLight pointLight) { return *pointLightData_ = pointLight; }
	SpotLight SetSpotLightProperty(SpotLight spotLight) { return *spotLightData_ = spotLight; }
	// cameradataの設定
	Vector3 SetCameraData(Vector3 camera) { return cameraData_->worldPosition = camera; }
	void SetEnvironmentTexture(uint32_t index) { index_ = index; }

#pragma endregion

private:
	Resource resource_{};
	PointLight* pointLightData_ = nullptr;
	SpotLight* spotLightData_ = nullptr;
	CameraData* cameraData_ = nullptr;
	Property property_{};
	Light light_{};
	uint32_t index_ = 0;
};