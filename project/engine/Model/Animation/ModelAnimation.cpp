#include "ModelAnimation.h"

void ModelAnimation::Initialize(const std::string& fileName)
{
	modelData_ = LoadGLTFFile("resources", fileName);

	animation_ = LoadAnimationFile("resources", fileName);

	skeleton_ = CreateSkeleton(modelData_.rootNode); // skeleton

	skinCluster_ = CreateSkinCluster(modelData_, skeleton_); // skinCluster

	CreateBuffer(); // bufferを作る
}

void ModelAnimation::SkeletonUpdate(Skeleton& skeleton)
{
	// すべてのjointを更新。
	for (Joint& joint : skeleton.joints) {
		joint.localMatrix = MakeAffineMatrix(joint.transform.scale, joint.transform.rotate, joint.transform.translate);

		if (joint.parent) {
			joint.skeletonSpaceMatrix = joint.localMatrix * skeleton.joints[*joint.parent].skeletonSpaceMatrix;
			
		}
		else {
			joint.skeletonSpaceMatrix = joint.localMatrix;
		
		}
	}
}

void ModelAnimation::SkinClusterUpdate(SkinCluster& skinCluster, const Skeleton& skeleton)
{
	for (size_t jointIndex = 0; jointIndex < skeleton.joints.size(); ++jointIndex) {
		assert(jointIndex < skinCluster.inverseBindPoseMatrices.size());
		skinCluster.mappedPalette[jointIndex].skeletonSpaceMatrix = 
			skinCluster.inverseBindPoseMatrices[jointIndex] * skeleton.joints[jointIndex].skeletonSpaceMatrix;
		skinCluster.mappedPalette[jointIndex].skeletonSpaceInverseTransposeMatrix =
			Transpose(Inverse(skinCluster.mappedPalette[jointIndex].skeletonSpaceMatrix));
    }
}

void ModelAnimation::Draw(bool isAnimation)
{
	// animationの適用
	if (isAnimation) {
		ApplyAnimation(skeleton_, animation_, animationTime_);
	}

	SkeletonUpdate(skeleton_); // skeletonの更新
	SkinClusterUpdate(skinCluster_, skeleton_); // skinClusterの更新

	D3D12_VERTEX_BUFFER_VIEW vbvs[2] = {
		VBV_,
		skinCluster_.influenceBufferView
	};
	
	DirectXCommon::GetCommandList()->IASetVertexBuffers(0, 2, vbvs); // VBVを設定
	DirectXCommon::GetCommandList()->IASetIndexBuffer(&IBV_);	
	DirectXCommon::GetCommandList()->SetGraphicsRootDescriptorTable(3, skinCluster_.paletteSrvHandle.second);
	
	// 描画。(DrawCall/ドローコール)。
	DirectXCommon::GetCommandList()->DrawIndexedInstanced(UINT(modelData_.indices.size()), 1, 0, 0, 0);
}

Animation ModelAnimation::LoadAnimationFile(const std::string& directoryPath, const std::string& fileName)
{
	Animation animation;
	Assimp::Importer importer;
	std::string filePath = directoryPath + "/" + fileName;
	const aiScene* scene = importer.ReadFile(filePath.c_str(), 0);
	assert(scene->mNumAnimations != 0);
	aiAnimation* animationAssimp = scene->mAnimations[0];
	animation.duration = float(animationAssimp->mDuration / animationAssimp->mTicksPerSecond);

	for (uint32_t channelIndex = 0; channelIndex < animationAssimp->mNumChannels; ++channelIndex) {
		aiNodeAnim* nodeAnimationAssimp = animationAssimp->mChannels[channelIndex];
		NodeAnimation& nodeAnimation = animation.nodeAnimations[nodeAnimationAssimp->mNodeName.C_Str()];
		// position
		for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumPositionKeys; ++keyIndex) {
			aiVectorKey& KeyAssimp = nodeAnimationAssimp->mPositionKeys[keyIndex];
			KeyframeVector3 keyframe;
			keyframe.time = float(KeyAssimp.mTime / animationAssimp->mTicksPerSecond);
			keyframe.value = { -KeyAssimp.mValue.x, KeyAssimp.mValue.y, KeyAssimp.mValue.z };
			nodeAnimation.translate.keyframes.push_back(keyframe);
		}
		// rotate
		for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumRotationKeys; ++keyIndex) {
			aiQuatKey& KeyAssimp = nodeAnimationAssimp->mRotationKeys[keyIndex];
			KeyframeQuaternion keyframe;
			keyframe.time = float(KeyAssimp.mTime / animationAssimp->mTicksPerSecond);
			keyframe.value = { KeyAssimp.mValue.x, -KeyAssimp.mValue.y, -KeyAssimp.mValue.z, KeyAssimp.mValue.w };
			nodeAnimation.rotate.keyframes.push_back(keyframe);
		}
		// scale
		for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumScalingKeys; ++keyIndex) {
			aiVectorKey& KeyAssimp = nodeAnimationAssimp->mScalingKeys[keyIndex];
			KeyframeVector3 keyframe;
			keyframe.time = float(KeyAssimp.mTime / animationAssimp->mTicksPerSecond);
			keyframe.value = { KeyAssimp.mValue.x, KeyAssimp.mValue.y, KeyAssimp.mValue.z };
			nodeAnimation.scale.keyframes.push_back(keyframe);
		}
	}
	// 解析終わり
	return animation;
}

//void ModelAnimation::PlayAnimation()
//{
//		
//	animationTime_ += 1.0f / 60.0f;
//	animationTime_ = std::fmod(animationTime_, animation_.duration);
//	NodeAnimation& rootNodeAnimation = animation_.nodeAnimations[modelData_.rootNode.name];
//	Vector3 translate = CalculateValue(rootNodeAnimation.translate.keyframes, animationTime_);
//	Quaternion rotate = CalculateValue(rootNodeAnimation.rotate.keyframes, animationTime_);
//	Vector3 scale = CalculateValue(rootNodeAnimation.scale.keyframes, animationTime_);
//	localMatrix_ = MakeAffineMatrix(scale, rotate, translate);
//}

Vector3 ModelAnimation::CalculateValue(const std::vector<KeyframeVector3>& keyframes, float time)
{
	assert(!keyframes.empty());
	if (keyframes.size() == 1 || time <= keyframes[0].time) {
		return keyframes[0].value;
	}

	for (size_t index = 0; index < keyframes.size() - 1; ++index) {
		size_t nextIndex = index + 1;
		if (keyframes[index].time <= time && time <= keyframes[nextIndex].time) {
			float t = (time - keyframes[index].time) / (keyframes[nextIndex].time - keyframes[index].time);
			return Lerp(keyframes[index].value, keyframes[nextIndex].value, t);
		}
	}

	return (*keyframes.rbegin()).value;
}

Quaternion ModelAnimation::CalculateValue(const std::vector<KeyframeQuaternion>& keyframes, float time)
{
	assert(!keyframes.empty());
	if (keyframes.size() == 1 || time <= keyframes[0].time) {
		return keyframes[0].value;
	}

	for (size_t index = 0; index < keyframes.size() - 1; ++index) {
		size_t nextIndex = index + 1;
		if (keyframes[index].time <= time && time <= keyframes[nextIndex].time) {
			float t = (time - keyframes[index].time) / (keyframes[nextIndex].time - keyframes[index].time);
			return Slerp(keyframes[index].value, keyframes[nextIndex].value, t);
		}
	}

	return (*keyframes.rbegin()).value;
	
}

int32_t ModelAnimation::CreateJoint(const Node& node, const std::optional<int32_t>& parent, std::vector<Joint>& joints)
{
	Joint joint;
	joint.name = node.name;
	joint.localMatrix = node.localMatrix;
	joint.skeletonSpaceMatrix = MakeIdentityMatrix();
	joint.transform = node.transform;
	joint.index = int32_t(joints.size());
	joint.parent = parent;
	joints.push_back(joint);
	for (const Node& child : node.children) {
		int32_t childIndex = CreateJoint(child, joint.index, joints);
		joints[joint.index].children.push_back(childIndex);
	}
	// 自身のindexを返す
	return joint.index;
}

Skeleton ModelAnimation::CreateSkeleton(const Node& rootNode)
{
	Skeleton skeleton;
	skeleton.root = CreateJoint(rootNode, {}, skeleton.joints);

	// 名前とindexのマッピングを行いアクセスしやすくする
	for (const Joint& joint : skeleton.joints) {
		skeleton.jointMap.emplace(joint.name, joint.index);
	}

	return skeleton;
}

SkinCluster ModelAnimation::CreateSkinCluster(const ModelData& modelData, const Skeleton& skeleton)
{
	SkinCluster skinCluster;
	// palette用のresouceを確保
	skinCluster.paletteResource = CreateResource::CreateBufferResource(sizeof(WellForGPU) * skeleton.joints.size());
	WellForGPU* mappedPalette = nullptr;
	skinCluster.paletteResource->Map(0, nullptr, reinterpret_cast<void**>(&mappedPalette));
	skinCluster.mappedPalette = { mappedPalette, skeleton.joints.size() }; // spanを使ってアクセスするようにする
	SrvManager::GetInstance()->ShiftIndex(); // srvのindexの位置を空いてる次にずらす
	srvIndex_ = SrvManager::GetInstance()->GetIndex();
	skinCluster.paletteSrvHandle.first = DescriptorManager::GetCPUDescriptorHandle(DescriptorManager::GetInstance()->GetSRV(), DescriptorManager::GetInstance()->GetDescSize().SRV, srvIndex_);
	skinCluster.paletteSrvHandle.second = DescriptorManager::GetGPUDescriptorHandle(DescriptorManager::GetInstance()->GetSRV(), DescriptorManager::GetInstance()->GetDescSize().SRV, srvIndex_);

	// palette用のsrvを作成。structuredBufferでアクセスできるようにする
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	srvDesc.Buffer.NumElements = UINT(skeleton.joints.size());
	srvDesc.Buffer.StructureByteStride = sizeof(WellForGPU);
	DirectXCommon::GetInstance()->GetDevice()->CreateShaderResourceView(skinCluster.paletteResource.Get(), &srvDesc, skinCluster.paletteSrvHandle.first);

	// influence用のResourceを確保。頂点ごとにinfluence情報を追加できるようにする
	skinCluster.influenceResource = CreateResource::CreateBufferResource(sizeof(VertexInfluence) * modelData.vertices.size());
	VertexInfluence* mappedInfluence = nullptr;
	skinCluster.influenceResource->Map(0, nullptr, reinterpret_cast<void**>(&mappedInfluence));
	std::memset(mappedInfluence, 0, sizeof(VertexInfluence) * modelData.vertices.size()); // 0埋め。weightを0にしておく
	skinCluster.mappedInfluence = { mappedInfluence, modelData.vertices.size() };

	// VertexBufferView
	skinCluster.influenceBufferView.BufferLocation = skinCluster.influenceResource->GetGPUVirtualAddress();
	// 使用するリソースのサイズは頂点サイズ
	skinCluster.influenceBufferView.SizeInBytes = UINT(sizeof(VertexInfluence) * modelData.vertices.size());
	// 1頂点あたりのサイズ
	skinCluster.influenceBufferView.StrideInBytes = sizeof(VertexInfluence);

	// InverseBindPoseMatrixを格納する場所を作成して、単位行列で埋める。
	skinCluster.inverseBindPoseMatrices.resize(skeleton.joints.size());
	std::generate(skinCluster.inverseBindPoseMatrices.begin(), skinCluster.inverseBindPoseMatrices.end(), MakeIdentityMatrix);

	for (const auto& jointWeight : modelData.skinClusterData) {
		auto it = skeleton.jointMap.find(jointWeight.first);
		if (it == skeleton.jointMap.end()) {
			continue;
		}
		// (*it).secondにはjointのindexが入っているので、該当のindexのinverseBindPoseMatrixを代入
		skinCluster.inverseBindPoseMatrices[(*it).second] = jointWeight.second.inverseBindPoseMatrix;
		for (const auto& vertexWeight : jointWeight.second.vertexWeights) {
			auto& currentInfluence = skinCluster.mappedInfluence[vertexWeight.vertexIndex];
			for (uint32_t index = 0; index < kNumMaxInfluence; ++index) {
				if (currentInfluence.weights[index] == 0.0f) {
					currentInfluence.weights[index] = vertexWeight.weight;
					currentInfluence.jointIndices[index] = (*it).second;
					break;
				}
			}
		}
	}

	return skinCluster;
}

void ModelAnimation::ApplyAnimation(Skeleton& skeleton, const Animation& animation, float animationTime)
{
	for (Joint& joint : skeleton.joints) {
		// 対象のjointのanimationがあれば、値の適用を行う。
		if (auto it = animation.nodeAnimations.find(joint.name); it != animation.nodeAnimations.end()) {
			const NodeAnimation& rootNodeAnimation = (*it).second;
			joint.transform.translate = CalculateValue(rootNodeAnimation.translate.keyframes, animationTime);
			joint.transform.rotate = CalculateValue(rootNodeAnimation.rotate.keyframes, animationTime);
			joint.transform.scale = CalculateValue(rootNodeAnimation.scale.keyframes, animationTime);

		}
	}
}

void ModelAnimation::CreateBuffer()
{
	// VertexResource
	resource_.vertexResource = CreateResource::CreateBufferResource(sizeof(VertexData) * modelData_.vertices.size());
	// VertexBufferView
	VBV_.BufferLocation = resource_.vertexResource->GetGPUVirtualAddress();
	// 使用するリソースのサイズは頂点サイズ
	VBV_.SizeInBytes = UINT(sizeof(VertexData) * modelData_.vertices.size());
	// 1頂点あたりのサイズ
	VBV_.StrideInBytes = sizeof(VertexData);

	// 頂点リソースにデータを書き込む
	VertexData* vertexData = nullptr;
	// 書き込むためのアドレスを取得
	resource_.vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	std::memcpy(vertexData, modelData_.vertices.data(), sizeof(VertexData) * modelData_.vertices.size()); // 頂点データをリソースにコピー

	// indexResource作成
	resource_.indexResource = CreateResource::CreateBufferResource(sizeof(uint32_t) * modelData_.indices.size());

	IBV_.BufferLocation = resource_.indexResource->GetGPUVirtualAddress();
	IBV_.SizeInBytes = sizeof(uint32_t) * UINT(modelData_.indices.size());
	IBV_.Format = DXGI_FORMAT_R32_UINT;

	uint32_t* index = nullptr;
	resource_.indexResource->Map(0, nullptr, reinterpret_cast<void**>(&index));
	std::memcpy(index, modelData_.indices.data(), sizeof(uint32_t) * modelData_.indices.size());

}
