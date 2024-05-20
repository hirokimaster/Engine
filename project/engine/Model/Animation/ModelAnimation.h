#pragma once
#include "engine/Model/Animation/Animation.h"
#include "engine/Model/Model.h"
#include "engine/DescriptorManager/DescriptorManager.h"
#include "engine/DescriptorManager/SRVManager/SrvManager.h"
#include  <numbers>

class ModelAnimation : public Model{
public:

	// 初期化
	void Initialize(const std::string& fileName);
	// 描画
	void Draw(bool isAnimation);

	// アニメーションを適用する
	void ApplyAnimation(Skeleton& skeleton, const Animation& animation, float animationTime);

	// animationの再生
	//void PlayAnimation();

	void SetAnimationTime(float animationTime) { animationTime_ = animationTime; }
	void SetTexHandle(uint32_t texHandle) { texHandle_ = texHandle; }
	void SetColor(const Vector4& color) { color_ = color; }

private: // ここでしか使わない関数

	// skeletonの更新
	void SkeletonUpdate(Skeleton& skeleton);
	// skinClusterの更新
	void SkinClusterUpdate(SkinCluster& skinCluster, const Skeleton& skeleton);

	// animation読み込み
	Animation LoadAnimationFile(const std::string& directoryPath, const std::string& fileName);

	// 任意の時刻の取得
	Vector3 CalculateValue(const std::vector<KeyframeVector3>& keyframes, float time);
	Quaternion CalculateValue(const std::vector<KeyframeQuaternion>& keyframe, float time);

	// nodeからjointを作る
	int32_t CreateJoint(const Node& node, const std::optional<int32_t>& parent, std::vector<Joint>& joints);
	// skeletonを作る
	Skeleton CreateSkeleton(const Node& rootNode);

	// skinClusterの生成
	SkinCluster CreateSkinCluster(const ModelData& modelData, const Skeleton& skeleton);

	// bufferを作る場所
	void CreateBuffer();

private:
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
	Skeleton skeleton_{};
	SkinCluster skinCluster_{};
	float animationTime_ = 0.0f;
};
