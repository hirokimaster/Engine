#pragma once
#include "engine/Math/Mathfunction.h"
#include "engine/Model/Model.h"
#include "engine/Transform/WorldTransform.h"
#include <vector>
#include <string>
#include <map>
#include <optional>

template<typename tValue>

struct Keyframe {
	float time;
	tValue value;
};

using KeyframeVector3 = Keyframe<Vector3>;
using KeyframeQuaternion = Keyframe<Quaternion>;

template<typename tValue>

struct AnimationCurve {
	std::vector<Keyframe<tValue>> keyframes;
};

struct NodeAnimation {
	AnimationCurve<Vector3> translate;
	AnimationCurve<Quaternion> rotate;
	AnimationCurve<Vector3> scale;
};

struct Animation {
	float duration;
	std::map<std::string, NodeAnimation> nodeAnimations;
};

struct Joint {
	QuaternionTransform transform;
	Matrix4x4 localMatrix;
	Matrix4x4 skeletonSpaceMatrix;
	std::string name;
	std::vector<int32_t> children;
	int32_t index;
	std::optional<int32_t> parent;
};

struct Skeleton {
	int32_t root;
	std::map<std::string, int32_t> jointMap;
	std::vector<Joint> joints;
};



class ModelAnimation : public Model{
public:

	// 初期化
	void Initialize(const std::string& fileName);
	// skeletonの更新
	void Update(Skeleton& skeleton);
	// 描画
	void Draw(WorldTransform& worldTransform, Camera& camera);

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
	Skeleton CreateSkeleton(const Node& rootNode);

	void SetTexHandle(uint32_t texHandle) { texHandle_ = texHandle; }
	void SetColor(const Vector4& color) { color_ = color; }

private:
	float animationTime_ = 0.0f;
	Animation animation_{};
	Matrix4x4 localMatrix_{};
	ModelData modelData_;
	Resource resource_ = {};
	D3D12_VERTEX_BUFFER_VIEW objVertexBufferView_{};
	Material* materialData_ = nullptr;
	DirectionalLight* directionalLightData_ = nullptr;
	uint32_t texHandle_ = 0;
	Vector4 color_ = {};
	Property property_{};
};
