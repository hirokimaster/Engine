/**
* @file ParticleSystem.h
* @brief パーティクルの初期化、生成を行うクラス(CPUParticle)
* @author 仁平 琉乃
*/

#pragma once
#pragma once
#include "engine/Graphics/Pipeline/GraphicsPipeline.h"
#include "engine/Graphics/CreateResource/CreateResource.h"
#include "engine/Graphics/Transform/WorldTransform.h"
#include "engine/Graphics/Camera/Camera.h"
#include "engine/Graphics/TextureManager/TextureManager.h"
#include "engine/Graphics/DescriptorManager/DescriptorManager.h"
#include "engine/3d/Model/ModelManager.h"
#include <random>
#include <numbers>
#include "engine/Input/Input.h"
#include "engine/Math/Mathfunction.h"

struct Particle {
	WorldTransform worldTransform;
	Vector3 velocity;
	Vector4 color;
	float lifeTime;
	float currentTime;
};

struct Emitter {
	WorldTransform worldransform; //!<エミッタのtransform
	uint32_t count; //!< 発生数
	float frequency; //!< 発生頻度
	float frequencyTime; //!< 頻度用時刻
};

struct AccelerationField {
	Vector3 acceleration; //!< 加速度
	AABB area; //!< 範囲
};

struct DistributionParam {
	Vector2 velocity;
	Vector2 color;
	Vector2 rotate;
	Vector2 scale;
	Vector2 lifeTime;
};

class ParticleSystem {
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ParticleSystem();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ParticleSystem();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// リソース作成
	/// </summary>
	void CreateBuffer();

	/// <summary>
	/// instancing用のSRVの作成
	/// </summary>
	void CreateSrv();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="worldTransform"></param>
	/// <param name="viewprojection"></param>
	void Draw(std::list<Particle>& partcles, const Camera& camera);

#pragma region setter

	void SetTexHandle(uint32_t texHandle) { texHandle_ = texHandle; }

	void SetNumInstance(uint32_t kNumMaxInstace) { kNumMaxInstance_ = kNumMaxInstace; }

	void SetModel(const std::string& fileName) { model_ = ModelManager::CreateObj(fileName); }

	void SetPosition(Vector3 position) { position_ = position; }

	void SetDistributionParam(const DistributionParam& param) { distParam_ = param; }

	void SetParticleParam(const Particle& param) { particle_ = param; }

#pragma endregion

	/// <summary>
	/// particleをランダム発生
	/// </summary>
	/// <param name="randomEngine"></param>
	/// <returns></returns>
	Particle MakeNewParticle(std::mt19937& randomEngine);

	/// <summary>
	/// emit関数
	/// </summary>
	/// <param name="emitter"></param>
	/// <param name="randomEngine"></param>
	/// <returns></returns>
	std::list<Particle> Emission(const Emitter& emitter, std::mt19937& randomEngine);

	/// <summary>
	/// 範囲
	/// </summary>
	/// <param name="aabb"></param>
	/// <param name="point"></param>
	/// <returns></returns>
	bool IsCollision(const AABB& aabb, const Vector3& point);

	/// <summary>
	/// ランダム
	/// </summary>
	/// <returns></returns>
	std::mt19937 random();

private:
	Resource resource_ = {};
	uint32_t kNumMaxInstance_ = 10000;
	descSize size_ = {};
	ModelData modelData_;
	Model* model_ = nullptr;
	D3D12_VERTEX_BUFFER_VIEW VertexBufferView_{};
	uint32_t texHandle_ = 0;
	ParticleForGPU* instancingData_ = nullptr;
	uint32_t index_ = 0;
	static Vector3 position_;
	Particle particle_{};
	DistributionParam distParam_{};
};
