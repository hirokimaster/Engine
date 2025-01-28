/**
* @file Object3DPlacer.h
* @brief モデルの種類決め、位置を管理クラス
* @author 仁平 琉乃
*/

#pragma once
#include "engine/ModelManager/ModelManager.h"
#include "engine/Lighting/Lighting.h"
#include "engine/Model/Animation/ModelAnimation.h"

class Object3DPlacer {
public:
	/// <summary>
	/// 普通のモデルの初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 行列の更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="worldTransform"></param>
	/// <param name="camera"></param>
	void Draw(Camera& camera);

	/// <summary>
	/// animation描画
	/// </summary>
	/// <param name="worldTransform"></param>
	/// <param name="camera"></param>
	/// <param name="isAnimation"></param>
	void Draw(Camera& camera, bool isAnimation);

#pragma region setter

	void SetModel(const std::string& fileName) { model_ = ModelManager::CreateObj(fileName); }
	void SetAnimModel(const std::string& fileName) { modelAnimation_ = ModelManager::Create(fileName); }
	void SetAnimationTime(float animationTime) { modelAnimation_->SetAnimationTime(animationTime); }
	void SetTexHandle(uint32_t texHandle) { texHandle_ = texHandle; }
	void SetColor(const Vector4& color) { materialData_->color = color; }
	void SetEnableLighting(const bool& enable) { materialData_->enableLighting = enable; }
	void SetLight(Lighting* lighting) { lighting_ = lighting; }
	void SetPosition(const Vector3& position) { worldTransform_.translate = position; }
	void SetRotate(const Vector3& rotate) { worldTransform_.rotate = rotate; }
	void SetScale(const Vector3& scale) { worldTransform_.scale = scale; }
	void SetParent(const WorldTransform* parent) { worldTransform_.parent = parent; }
	Material SetMaterialProperty(const Material& materialdata) { return *materialData_ = materialdata; }
	// directionalLightの設定
	DirectionalLight SetLightingProperty(DirectionalLight directionalLight) { return *directionalLightData_ = directionalLight; }
	void SetUVTransform(const UVTransform& uvTransform) { uvTransform_ = uvTransform; }

#pragma endregion

#pragma region getter

	const WorldTransform& GetWorldTransform() { return worldTransform_; }

#pragma endregion

private:

	void CreateUVTransformMatrix();

private:
	Model* model_ = nullptr;
	ModelAnimation* modelAnimation_ = nullptr;
	Resource resource_ = {};
	Material* materialData_ = nullptr;
	DirectionalLight* directionalLightData_ = nullptr;
	GraphicsPipelineData pipelineData_{};
	uint32_t texHandle_ = 0;
	Vector4 color_ = {};
	Lighting* lighting_{};
	WorldTransform worldTransform_{};
	UVTransform uvTransform_{
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
	};
};
