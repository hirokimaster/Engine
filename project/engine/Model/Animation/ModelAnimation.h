#pragma once
#include "engine/Model/Animation/Animation.h"
#include "engine/Model/Model.h"
#include "engine/DescriptorManager/DescriptorManager.h"
#include "engine/DescriptorManager/SRVManager/SrvManager.h"

class ModelAnimation : public Model{
public:

	// 初期化
	void Initialize(const std::string& fileName);
	// skeletonの更新
	void Update(Skeleton& skeleton);
	// skinClusterの更新
	void Update(SkinCluster& skinCluster, const Skeleton& skeleton);
	// 描画
	void Draw(WorldTransform& worldTransform, Camera& camera, SkinCluster& skinCluster);

	// animation読み込み
	Animation LoadAnimationFile(const std::string& directoryPath, const std::string& fileName);
	// animationの再生
	void PlayAnimation();
	// 任意の時刻の取得
	Vector3 CalculateValue(const std::vector<KeyframeVector3>& keyframes, float time);
	Quaternion CalculateValue(const std::vector<KeyframeQuaternion>& keyframe, float time);

	// nodeからjointを作る
	int32_t CreateJoint(const Node& node, const std::optional<int32_t>& parent, std::vector<Joint>& joints);
	// skeletonを作る
	Skeleton CreateSkeleton();

	// skinClusterの生成
	SkinCluster CreateSkinCluster(const Skeleton& skeleton);

	// アニメーションを適用する
	void ApplyAnimation(Skeleton& skeleton, float animationTime);

	void SetTexHandle(uint32_t texHandle) { texHandle_ = texHandle; }
	void SetColor(const Vector4& color) { color_ = color; }

private:
	float animationTime_ = 0.0f;
	Animation animation_{};
	Matrix4x4 localMatrix_{};
	ModelData modelData_;
	Resource resource_ = {};
	D3D12_VERTEX_BUFFER_VIEW VBV_{};
	Material* materialData_ = nullptr;
	DirectionalLight* directionalLightData_ = nullptr;
	uint32_t texHandle_ = 0;
	Vector4 color_ = {};
	Property property_{};
	D3D12_INDEX_BUFFER_VIEW IBV_{};
	uint32_t srvIndex_ = 0;
};
