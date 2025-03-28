#pragma once
#include "engine/Graphics/Effects/Particle/ParticleEditor.h"
#include "engine/Graphics/Effects/Particle/GPUParticle.h"

using std::string;
using std::map;
using std::unique_ptr;
using std::make_unique;
using std::pair;
using std::unordered_map;
using std::vector;

class ParticleManager {
public:

	/// <summary>
	/// インスタンス取得
	/// </summary>
	/// <returns></returns>
	static ParticleManager* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// editorを起動する
	/// </summary>
	/// <param name="particleName"></param>
	void CreateParam(const char* particleName);

	/// <summary>
	/// particleの情報をエディターから読み込む
	/// </summary>
	/// <param name="particleName"></param>
	void ApplyParticleInfo(const char* particleName);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// editorの更新
	/// </summary>
	void UpdateEditor();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const Camera& camera);

#pragma region getter

	GPUParticle* GetParticle(const string& name);

#pragma endregion

private:

	/// <summary>
	/// paticleのインスタンス生成
	/// </summary>
	/// <param name="particleName"></param>
	void Create();

	/// <summary>
	/// 使い終わったインスタンスをキューに戻す
	/// </summary>
	void Push(GPUParticle* particle);

private:
	ParticleManager() = default;
	~ParticleManager() = default;
	ParticleManager(const ParticleManager&) = delete;
	ParticleManager& operator=(const ParticleManager&) = delete;

private:
	// particleのパラメータ保存用のコンテナ
	map<string, EmitterSphere> params_;
	// 使用可能なエフェクトのポインタ
	std::queue<GPUParticle*> pool_;
	// インスタンスを入れておくコンテナ
	vector<unique_ptr<GPUParticle>> particles_;
	// 同時に存在できるparticleの数
	static const uint32_t kPoolSize = 30;
};