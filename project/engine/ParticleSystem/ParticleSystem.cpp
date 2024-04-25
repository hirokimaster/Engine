#include "ParticleSystem.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="filename"></param>
void ParticleSystem::Initialize(const std::string& filename) {

	model_ = std::make_unique<Model>();
	modelData_ = model_->LoadObjFile("resources", filename);
	// リソース作成
	CreateResource(modelData_);
	// instancing用のSRV作成
	CreateSrv();

}

/// <summary>
/// リソース作成
/// </summary>
/// <param name="modelData"></param>
void ParticleSystem::CreateResource(ModelData modelData) {

	// Instancing用のTransformationMatrixResourceを作る
	resource_.instancingResource = CreateResource::CreateBufferResource(sizeof(ParticleForGPU) * kNumMaxInstance_);
	// 書き込むためのアドレスを取得
	resource_.instancingResource->Map(0, nullptr, reinterpret_cast<void**>(&instancingData_));
	// 単位行列を書き込む
	for (uint32_t index = 0; index < kNumMaxInstance_; ++index) {
		instancingData_[index].WVP = MakeIdentityMatrix();
		instancingData_[index].World = MakeIdentityMatrix();
		instancingData_[index].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	}

	// VertexResource
	resource_.vertexResource = CreateResource::CreateBufferResource(sizeof(VertexData) * modelData.vertices.size());
	// VertexBufferView
	VertexBufferView_.BufferLocation = resource_.vertexResource->GetGPUVirtualAddress();
	// 使用するリソースのサイズは頂点サイズ
	VertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * modelData.vertices.size());
	// 1頂点あたりのサイズ
	VertexBufferView_.StrideInBytes = sizeof(VertexData);

	// 頂点リソースにデータを書き込む
	VertexData* vertexData = nullptr;
	// 書き込むためのアドレスを取得
	resource_.vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	std::memcpy(vertexData, modelData.vertices.data(), sizeof(VertexData) * modelData.vertices.size()); // 頂点データをリソースにコピー
	resource_.materialResource = CreateResource::CreateBufferResource(sizeof(Material));
	// データを書き込む
	Material* materialData = nullptr;
	// アドレスを取得
	resource_.materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	materialData->color = { 1.0f,1.0f,1.0f,1.0f };
}

/// <summary>
/// instancing用のSRV作成
/// </summary>
void ParticleSystem::CreateSrv() {
	// srvの位置をtextureのsrvの位置から設定する
	index_ = SrvManager::GetInstance()->GetIndex();
	index_++;
	SrvManager::GetInstance()->SetIndex(index_);
	SrvManager::GetInstance()->CreateInstancingSrv(resource_, index_);
}

/// <summary>
/// 描画
/// </summary>
/// <param name="worldTransform"></param>
/// <param name="viewprojection"></param>
void ParticleSystem::Draw(std::list<Particle>& particles, const Camera& camera) {

	uint32_t  numInstance = 0; // 描画すべきインスタンス数

	Matrix4x4 backToFrontMatrix = MakeRotateYMatrix(std::numbers::pi_v<float>);

	Matrix4x4 cameraMatrix = Inverse(camera.matView);

	Matrix4x4 billboardMatrix = Multiply(backToFrontMatrix, cameraMatrix);
	billboardMatrix.m[3][0] = 0.0f;
	billboardMatrix.m[3][1] = 0.0f;
	billboardMatrix.m[3][2] = 0.0f;

	for (std::list<Particle>::iterator particleItr = particles.begin(); particleItr != particles.end();) {
		if ((*particleItr).lifeTime <= (*particleItr).currentTime) {
			particleItr = particles.erase(particleItr);
			continue;
		}

		Matrix4x4 worldMatrix = MakeBiilboardWorldMatrix((*particleItr).worldTransform.scale, billboardMatrix,
			(*particleItr).worldTransform.translate);
		Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(camera.matView, camera.matProjection));
		(*particleItr).currentTime += 1.0f / 60.0f;
		float alpha = 1.0f - ((*particleItr).currentTime / (*particleItr).lifeTime);

		if (numInstance < kNumMaxInstance_) {
			instancingData_[numInstance].WVP = worldViewProjectionMatrix;
			//instancingData_[numInstance].World = worldMatrix;
			instancingData_[numInstance].color = (*particleItr).color;
			instancingData_[numInstance].color.w = alpha;
			++numInstance;
		}

		++particleItr;
	}

	Property property = GraphicsPipeline::GetInstance()->GetPSO().Particle;

	// Rootsignatureを設定。PSOに設定してるけど別途設定が必要
	DirectXCommon::GetCommandList()->SetGraphicsRootSignature(property.rootSignature_.Get());
	DirectXCommon::GetCommandList()->SetPipelineState(property.graphicsPipelineState_.Get()); // PSOを設定
	DirectXCommon::GetCommandList()->IASetVertexBuffers(0, 1, &VertexBufferView_); // VBVを設定
	// 形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけば良い
	DirectXCommon::GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// マテリアルCBufferの場所を設定
	DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(0, resource_.materialResource->GetGPUVirtualAddress());
	// instancing用のCBufferの場所を設定
	DirectXCommon::GetCommandList()->SetGraphicsRootDescriptorTable(1, SrvManager::GetInstance()->GetInstancingGPUHandle(index_));
	DirectXCommon::GetCommandList()->SetGraphicsRootDescriptorTable(2, SrvManager::GetInstance()->GetGPUHandle(texHandle_));
	// 描画。(DrawCall/ドローコール)。
	DirectXCommon::GetCommandList()->DrawInstanced(UINT(modelData_.vertices.size()), numInstance, 0, 0);
}

/// <summary>
/// particleをランダムで発生
/// </summary>
/// <param name="randomEngine"></param>
/// <returns></returns>
Particle ParticleSystem::MakeNewParticle(std::mt19937& randomEngine) {
	std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);
	std::uniform_real_distribution<float> distColor(0.0f, 1.0f);
	std::uniform_real_distribution<float> distTime(1.0f, 3.0f);
	Particle particle;
	particle.worldTransform.Initialize();
	particle.worldTransform.scale = { 1.0f, 1.0f, 1.0f };
	particle.worldTransform.rotate = { 0.0f,0.0f,0.0f };
	particle.worldTransform.translate = { distribution(randomEngine),  distribution(randomEngine) , distribution(randomEngine) };
	particle.velocity = { distribution(randomEngine) , distribution(randomEngine) , distribution(randomEngine) };
	particle.color = { distColor(randomEngine) , distColor(randomEngine) , distColor(randomEngine), 1.0f };
	particle.lifeTime = distTime(randomEngine);
	particle.currentTime = 0;
	return particle;
}

std::list<Particle> ParticleSystem::Emission(const Emitter& emitter, std::mt19937& randomEngine) {
	std::list<Particle> particles;
	for (uint32_t count = 0; count < emitter.count; ++count) {
		particles.push_back(MakeNewParticle(randomEngine));
	}

	return particles;

}

bool ParticleSystem::IsCollision(const AABB& aabb, const Vector3& point)
{
	if (point.x >= aabb.min.x && point.x <= aabb.max.x && point.y >= aabb.min.y && point.y <= aabb.max.y
		&& point.z >= aabb.min.z && point.z <= aabb.max.z) {
		return true;
	}

	return false;
}

std::mt19937 ParticleSystem::random()
{
	std::random_device seed;
	std::mt19937 randomEngine(seed());

	return randomEngine;
}