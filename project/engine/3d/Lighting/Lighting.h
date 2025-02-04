/**
* @file Lighting.h
* @brief オブジェクトへのライティングの適用
* @author 仁平 琉乃
*/

#pragma once
#include "engine/3d/Model/Model.h"
#include "engine/Graphics/DescriptorManager/SRVManager/SrvManager.h"
#include "engine/Graphics/CreateResource/CreateResource.h"
#include "engine/Graphics/Pipeline/GraphicsPipeline.h"
#include "engine/Graphics/Camera/Camera.h"

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
	Light light_{};
	uint32_t index_ = 0;
};