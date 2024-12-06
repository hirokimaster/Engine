#pragma once
#include "engine/ParticleSystem/ParticleEditor.h"
#include "engine/ParticleSystem/GPUParticle.h"

using std::string;
using std::map;
using std::unique_ptr;
using std::make_unique;
using std::pair;

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
	void ApplyParticleInfo(const char* particleName);

	/// <summary>
	/// paticleのインスタンス生成
	/// </summary>
	/// <param name="particleName"></param>
	void CreateParticle(const string& particleName, const string& model, uint32_t texHandle);

	/// <summary>
	/// particleの名前を指定して更新
	/// </summary>
	/// <param name="particleName"></param>
	void Update(const string& particleName);

	/// <summary>
	/// editorの更新
	/// </summary>
	void UpdateEditor();

	/// <summary>
	/// particleの名前を指定して描画
	/// </summary>
	/// <param name="particleName"></param>
	/// <param name="camera"></param>
	void Draw(const string& particleName, const Camera& camera);

#pragma region setter

	void SetColor(const string& particleName, const Vector4& color) { particles_[particleName]->SetColor(color); }

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
	map<string, unique_ptr<GPUParticle>> particles_;

};