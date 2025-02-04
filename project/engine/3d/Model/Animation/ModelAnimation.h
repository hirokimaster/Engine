/**
* @file ModelAnimation.h
* @brief アニメーションを含むGLTFモデルの読み込み、配置
* @author 仁平 琉乃
*/

#pragma once
#include "engine/3d/Model/Animation/Animation.h"
#include "engine/3d/Model/Model.h"
#include "engine/Graphics/DescriptorManager/DescriptorManager.h"
#include "engine/Graphics/DescriptorManager/SRVManager/SrvManager.h"
#include  <numbers>

class ModelAnimation : public Model{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="fileName"></param>
	void Initialize(const std::string& fileName);
	
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="isAnimation"></param>
	void Draw(bool isAnimation);

	/// <summary>
	/// アニメーションを適用する
	/// </summary>
	/// <param name="skeleton"></param>
	/// <param name="animation"></param>
	/// <param name="animationTime"></param>
	void ApplyAnimation(Skeleton& skeleton, const Animation& animation, float animationTime);

	// animationの再生
	//void PlayAnimation();

#pragma region setter

	void SetAnimationTime(float animationTime) { animationTime_ = animationTime; }
	void SetTexHandle(uint32_t texHandle) { texHandle_ = texHandle; }
	void SetColor(const Vector4& color) { color_ = color; }

#pragma endregion

private: // ここでしか使わない関数

	/// <summary>
	/// スケルトンの更新
	/// </summary>
	/// <param name="skeleton"></param>
	void SkeletonUpdate(Skeleton& skeleton);
	
	/// <summary>
	/// SkinClusterの更新
	/// </summary>
	/// <param name="skinCluster"></param>
	/// <param name="skeleton"></param>
	void SkinClusterUpdate(SkinCluster& skinCluster, const Skeleton& skeleton);

	/// <summary>
	/// アニメーション読み込み
	/// </summary>
	/// <param name="directoryPath"></param>
	/// <param name="fileName"></param>
	/// <returns></returns>
	Animation LoadAnimationFile(const std::string& directoryPath, const std::string& fileName);

	/// <summary>
	/// 任意の時刻取得(Vector3)
	/// </summary>
	/// <param name="keyframes"></param>
	/// <param name="time"></param>
	/// <returns></returns>
	Vector3 CalculateValue(const std::vector<KeyframeVector3>& keyframes, float time);

	/// <summary>
	/// 任意の時刻取得(Quaternion)
	/// </summary>
	/// <param name="keyframe"></param>
	/// <param name="time"></param>
	/// <returns></returns>
	Quaternion CalculateValue(const std::vector<KeyframeQuaternion>& keyframe, float time);

	/// <summary>
	/// ノードからジョイントを作る
	/// </summary>
	/// <param name="node"></param>
	/// <param name="parent"></param>
	/// <param name="joints"></param>
	/// <returns></returns>
	int32_t CreateJoint(const Node& node, const std::optional<int32_t>& parent, std::vector<Joint>& joints);
	
	/// <summary>
	/// スケルトンを作る
	/// </summary>
	/// <param name="rootNode"></param>
	/// <returns></returns>
	Skeleton CreateSkeleton(const Node& rootNode);

	/// <summary>
	/// SkinClusterを作る
	/// </summary>
	/// <param name="modelData"></param>
	/// <param name="skeleton"></param>
	/// <returns></returns>
	SkinCluster CreateSkinCluster(const ModelData& modelData, const Skeleton& skeleton);

	/// <summary>
	/// BufferResource作成まとめ
	/// </summary>
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
	D3D12_INDEX_BUFFER_VIEW IBV_{};
	uint32_t srvIndex_ = 0;
	Skeleton skeleton_{};
	SkinCluster skinCluster_{};
	float animationTime_ = 0.0f;
	Microsoft::WRL::ComPtr<ID3D12Resource> uavResource_ = nullptr;
};
