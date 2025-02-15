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
	void StartEditor(const char* particleName);

	/// <summary>
	/// particleの情報をエディターから読み込む
	/// </summary>
	/// <param name="particleName"></param>
	void ApplyParticleInfo(const char* particleName, uint32_t id = 0);

	/// <summary>
	/// paticleのインスタンス生成
	/// </summary>
	/// <param name="particleName"></param>
	void CreateParticle(const string& particleName, const string& model, uint32_t texHandle);

	/// <summary>
	/// particleの名前を指定して更新
	/// </summary>
	/// <param name="particleName"></param>
	void Update(const string& particleName, uint32_t id = 0);

	/// <summary>
	/// editorの更新
	/// </summary>
	void UpdateEditor();

	/// <summary>
	/// particleの名前を指定して描画
	/// </summary>
	/// <param name="particleName"></param>
	/// <param name="camera"></param>
	void Draw(const string& particleName, const Camera& camera, uint32_t id = 0);

	/// <summary>
	/// コンテナから削除
	/// </summary>
	/// <param name="particleName"></param>
	/// <param name="id"></param>
	void Clear(const string& particleName, uint32_t id);

#pragma region setter

	void SetColor(const string& particleName, const Vector4& color) {
		for (auto& particle : particles_[particleName]) {
			particle->SetColor(color);
		}
	}

	void SetPosition(const string& particleName, const Vector3& position, uint32_t id = 0) { particles_[particleName][id]->SetPosition(position); }

#pragma endregion


private:
	ParticleManager() = default;
	~ParticleManager() = default;
	ParticleManager(const ParticleManager&) = delete;
	ParticleManager& operator=(const ParticleManager&) = delete;

private:
	// particleのパラメータ保存用のコンテナ
	map<string, EmitterSphere> params_;
	// particleのインスタンス保存用のコンテナ
	unordered_map<string, vector<unique_ptr<GPUParticle>>> particles_;

};