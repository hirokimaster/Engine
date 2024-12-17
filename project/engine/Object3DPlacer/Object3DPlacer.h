/**
* @file Object3DPlacer.h
* @brief モデルの位置決め、配置を行うクラス
* @author 仁平 琉乃
*/

#pragma once
#include "engine/ModelManager/ModelManager.h"
#include "engine/Lighting/Lighting.h"
#include "engine/Model/Animation/ModelAnimation.h"

class Object3DPlacer {
public:

	Object3DPlacer();
	~Object3DPlacer();

	/// <summary>
	/// 普通のモデルの初期化
	/// </summary>
	void Initialize(bool isInstancing = false, uint32_t numInstance = 0);

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

private:

	/// <summary>
	/// インスタンシング用の初期化
	/// </summary>
	void InitializeInstancing();

	/// <summary>
	/// インスタンシングの更新
	/// </summary>
	void UpdateInstancing();

	/// <summary>
	/// pipeline設定
	/// </summary>
	void CreatePipeline();

#pragma region setter

	void SetModel(const std::string& fileName) { model_ = ModelManager::CreateObj(fileName); }
	void SetAnimModel(const std::string& fileName) { modelAnimation_ = ModelManager::Create(fileName); }
	void SetAnimationTime(float animationTime) { modelAnimation_->SetAnimationTime(animationTime); }
	void SetTexHandle(uint32_t texHandle) { texHandle_ = texHandle; }
	void SetColor(const Vector4& color) { materialData_->color = color; }
	void SetEnableLighting(const bool& enable) { materialData_->enableLighting = enable; }
	void SetLight(Lighting* lighting) { lighting_ = lighting; }
	void SetWorldTransform(WorldTransform worldTransform) { worldTransform_ = worldTransform; }
	void SetPosition(Vector3 position) { worldTransform_.translate = position; }
	void SetRotate(Vector3 rotate) { worldTransform_.rotate = rotate; }
	void SetScale(Vector3 scale) { worldTransform_.scale = scale; }
	void SetNumInstance(uint32_t instance) { numInstance_ = instance; }
	WorldTransform& GetWorldTransform() { return worldTransform_; }
	Material SetMaterialProperty(Material materialdata) { return *materialData_ = materialdata; }
	// directionalLightの設定
	DirectionalLight SetLightingProperty(DirectionalLight directionalLight) { return *directionalLightData_ = directionalLight; }
	void SetUVTransform(UVTransform uvTransform) { uvTransform_ = uvTransform; }

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
	// インスタンシングにするかどうか
	bool isInstancing_ = false;
	uint32_t numInstance_ = 0;
	ConstBufferDataWorldTransform* instancingData_ = nullptr;
	uint32_t srvIndex_ = 0;
	std::vector<WorldTransform> worldTransformInstancing_;
};
