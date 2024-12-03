#pragma once
#include "engine/ParticleSystem/ParticleEditor.h"
#include "engine/ParticleSystem/GPUParticle.h"

using std::string;
using std::map;
using std::unique_ptr;
using std::make_unique;

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
	void StartEditor(const string& particleName);

	/// <summary>
	/// particleの情報をエディターから読み込む
	/// </summary>
	/// <param name="particleName"></param>
	void ApplyParticleInfo(const char* particleName);

	/// <summary>
	/// paticleのインスタンス生成
	/// </summary>
	/// <param name="particleName"></param>
	void CreateParticle(const string& particleName);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

private:


private:
	// particleのパラメータ保存用のコンテナ
	map<string, EmitterSphere> params_;
	// editor
	unique_ptr<ParticleEditor> editor_ = nullptr;

};