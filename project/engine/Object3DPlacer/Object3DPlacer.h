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
	/// 描画
	/// </summary>
	/// <param name="worldTransform"></param>
	/// <param name="camera"></param>
	void Draw(WorldTransform worldTransform, Camera& camera);

	/// <summary>
	/// animation描画
	/// </summary>
	/// <param name="worldTransform"></param>
	/// <param name="camera"></param>
	/// <param name="isAnimation"></param>
	void Draw(WorldTransform worldTransform, Camera& camera, bool isAnimation);

	// setter
	void SetModel(const std::string& fileName) { model_ = ModelManager::CreateObj(fileName); }
	void SetAnimModel(const std::string& fileName) { modelAnimation_ = ModelManager::Create(fileName); }
	void SetAnimationTime(float animationTime) { modelAnimation_->SetAnimationTime(animationTime); }
	void SetTexHandle(uint32_t texHandle) { texHandle_ = texHandle; }
	void SetColor(const Vector4& color) { color_ = color; }
	void SetLight(Lighting* lighting) { lighting_ = lighting; }
	Material SetMaterialProperty(Material materialdata) { return *materialData_ = materialdata; }
	// directionalLightの設定
	DirectionalLight SetLightingProperty(DirectionalLight directionalLight) { return *directionalLightData_ = directionalLight; }


private:
	Model* model_ = nullptr;
	ModelAnimation* modelAnimation_ = nullptr;
	Resource resource_ = {};
	Material* materialData_ = nullptr;
	DirectionalLight* directionalLightData_ = nullptr;
	Property property_{};
	uint32_t texHandle_ = 0;
	Vector4 color_ = {};
	Lighting* lighting_{};
};
