/**
* @file ParticleSystem.cpp
* @brief パーティクルの初期化、生成を行うクラス(CPUParticle)
* @author 仁平 琉乃
*/

#include "ParticleSystem.h"

ParticleSystem::ParticleSystem()
{
}

ParticleSystem::~ParticleSystem()
{
	// デストラクタが呼ばれたらinstancing用のSRVがあるとこ解放する
	SrvManager::GetInstance()->StructuredBufIndexFree(index_);
}

Vector3 ParticleSystem::position_;

/// <summary>
/// 初期化
/// </summary>
void ParticleSystem::Initialize() {

	// リソース作成
	CreateBuffer();
	// instancing用のSRV作成
	CreateSrv();
}

/// <summary>
/// リソース作成
/// </summary>
void ParticleSystem::CreateBuffer() {

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
	SrvManager::GetInstance()->StructuredBufIndexAllocate();
	index_ = SrvManager::GetInstance()->GetStructuredBufIndex();
	SrvManager::GetInstance()->CreateInstancingSrv(resource_.instancingResource, index_);
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

	GraphicsPipelineData pipelineData = GraphicsPipeline::GetInstance()->GetPSO().Particle;

	// Rootsignatureを設定。PSOに設定してるけど別途設定が必要
	DirectXCommon::GetCommandList()->SetGraphicsRootSignature(pipelineData.rootSignature_.Get());
	DirectXCommon::GetCommandList()->SetPipelineState(pipelineData.graphicsPipelineState_.Get()); // PSOを設定
	// 形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけば良い
	DirectXCommon::GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// マテリアルCBufferの場所を設定
	DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(0, resource_.materialResource->GetGPUVirtualAddress());
	// instancing用のCBufferの場所を設定
	SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(1, index_); // structuredBuffer
	SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(2, texHandle_); // texture
	
	// モデルがあるなら描画する
	if (model_) {
		model_->Draw(numInstance);
	}
}

/// <summary>
/// particleをランダムで発生
/// </summary>
/// <param name="randomEngine"></param>
/// <returns></returns>
Particle ParticleSystem::MakeNewParticle(std::mt19937& randomEngine) {
	std::uniform_real_distribution<float> distVelocity(distParam_.velocity.x, distParam_.velocity.y);
	std::uniform_real_distribution<float> distColor(distParam_.color.x, distParam_.color.y);
	std::uniform_real_distribution<float> distRotate(distParam_.rotate.x, distParam_.rotate.y);
	std::uniform_real_distribution<float> distScale(distParam_.scale.x, distParam_.scale.y);
	std::uniform_real_distribution<float> distTime(distParam_.lifeTime.x, distParam_.lifeTime.y);
	Particle particle;
	particle_.worldTransform.Initialize();
	particle_.worldTransform.scale = {distScale(randomEngine),distScale(randomEngine) ,distScale(randomEngine)};
	particle_.worldTransform.rotate = { distRotate(randomEngine),distRotate(randomEngine) ,distRotate(randomEngine) };
	particle_.worldTransform.translate = position_;
	particle_.velocity = { distVelocity(randomEngine), distVelocity(randomEngine) , distVelocity(randomEngine) };
	particle_.color = { distColor(randomEngine),distColor(randomEngine),distColor(randomEngine), distColor(randomEngine) };
	particle_.lifeTime = distTime(randomEngine);
	particle_.currentTime = 0;

	particle = particle_;
	
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