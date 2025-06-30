/**
* @file Object3DPlacer.h
* @brief モデルの配置を行うクラス
* @author 仁平 琉乃
*/

#pragma once
#include "engine/3d/Model/ModelManager.h"
#include "engine/3d/Lighting/Lighting.h"
#include "engine/3d/Model/Animation/ModelAnimation.h"
#include "engine/Graphics/Transform/WorldTransform.h"
#include "engine/Graphics/Transform/InstanceWorldTransform.h"

// カテゴリ
enum class DrawCategory {
	Background,
	World,
	Foreground,
	UI
};

struct Object3dInstancing {
	InstanceWorldTransform worldTransform;
	Vector4 color;
	uint32_t texHandel;
	bool isAlive;
	DrawCategory category = DrawCategory::World;
};

class Object3dPlacer {
public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Object3dPlacer();

	/// <summary>
	/// 普通のモデルの初期化
	/// </summary>
	void Initialize(bool isInstancing = false);

	/// <summary>
	/// 行列の更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="worldTransform"></param>
	/// <param name="camera"></param>
	void Draw(const Camera& camera);

	/// <summary>
	/// animation描画
	/// </summary>
	/// <param name="worldTransform"></param>
	/// <param name="camera"></param>
	/// <param name="isAnimation"></param>
	void Draw(const Camera& camera, bool isAnimation);

#pragma region setter

	void SetModel(const std::string& fileName) { model_ = ModelManager::CreateObj(fileName); }
	void SetAnimModel(const std::string& fileName) { modelAnimation_ = ModelManager::Create(fileName); }
	void SetAnimationTime(float animationTime) { modelAnimation_->SetAnimationTime(animationTime); }
	void SetTexHandle(uint32_t texHandle) { texHandle_ = texHandle; }
	void SetColor(const Vector4& color) { materialData_->color = color; }
	void SetEnableLighting(bool enable) { materialData_->enableLighting = enable; }
	void SetLight(Lighting* lighting) { lighting_ = lighting; }
	void SetPosition(const Vector3& position) { worldTransform_.translate = position; }
	void SetRotate(const Vector3& rotate) { worldTransform_.rotate = rotate; }
	void SetScale(const Vector3& scale) { worldTransform_.scale = scale; }
	void SetParent(const WorldTransform* parent) { worldTransform_.parent = parent; }
	void SetMatWorld(const Matrix4x4& mat) { worldTransform_.matWorld = mat; worldTransform_.TransferMatrix(); }
	void SetShadow(bool shadow) { shadow_ = shadow; }
	Material SetMaterialProperty(const Material& materialdata) { return *materialData_ = materialdata; }
	// directionalLightの設定
	DirectionalLight SetLightingProperty(const DirectionalLight& directionalLight) { return *directionalLightData_ = directionalLight; }
	void SetUVTransform(const UVTransform& uvTransform) { uvTransform_ = uvTransform; }
	void SetObject3dInstancing(const std::shared_ptr<Object3dInstancing>& data) { object3dInstancing_.push_back(data); }
	void SetCategory(const DrawCategory& category) { category_ = category; }

#pragma endregion

#pragma region getter

	const WorldTransform& GetWorldTransform() { return worldTransform_; }

	const std::vector<std::shared_ptr<Object3dInstancing>>& GetObject3dInstancing() { return object3dInstancing_; }

	float GetPositionZ()const { return worldTransform_.translate.z; }

	const DrawCategory& GetCategory()const { return category_; }
	
#pragma endregion

private:

	void CreateUVTransformMatrix();

	void CreateInstancingBuffer();

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
	Microsoft::WRL::ComPtr<ID3D12Resource> instancingResource_;
	InstanceWorldTransformForGPU* instancingData_ = nullptr;
	uint32_t srvIndex_ = 0;
	uint32_t numInstance_ = 0;
	static const uint32_t kMaxInstance_ = 1000;
	std::vector<std::shared_ptr<Object3dInstancing>> object3dInstancing_;
	bool isInstancing_;
	DrawCategory category_ = DrawCategory::World;
	bool shadow_ = false;
};
