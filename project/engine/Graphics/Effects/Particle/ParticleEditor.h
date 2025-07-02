/**
* @file ParticleEditor.h
* @brief jsonからparamを読み込む
* @author 仁平 琉乃
*/

#pragma once
#include <variant>
#include "engine/Math/Vector3.h"
#include <map>
#include <string>
#include <filesystem>
#include <iostream>
#include <fstream>
#include "engine/Utility/ImGuiManager/ImGuiManager.h"
#include "externals/Json/json.hpp"
#include <Windows.h>
#include "engine/Graphics/Effects/Particle/GPUParticle.h"
#include "engine/Graphics/TextureManager/TextureManager.h"
#include <memory>

using std::variant;
using std::map;
using std::string;

class ParticleEditor {
public:

	/// <summary>
	/// インスタンス取得
	/// </summary>
	/// <returns></returns>
	static ParticleEditor* GetInstance();

	// パラメーター
	using Param = variant<int32_t, float, Vector3>;

	// particle
	using Particle = map<string, Param>;

	// グローバル変数の保存先ファイルパス
	const string kDirectoryPath = "resources/ParticleParam/";

	/// <summary>
	/// パーティクルの作成
	/// </summary>
	/// <param name="groupName"></param>
	void CreateParticle(const string& particleName);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// ディレクトリの全ファイル読み込み
	/// </summary>
	void LoadFiles();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const Camera& camera);

private:
	/// <summary>
    /// ファイルに書き出し
    /// </summary>
    /// <param name="groupName"></param>
	void SaveFile(const std::string& particleName);

	/// <summary>
	/// ファイルから読み込む
	/// </summary>
	/// <param name="groupName"></param>
	void LoadFile(const string& particleName);

	/// <summary>
	/// 変換
	/// </summary>
	/// <param name="particle"></param>
	/// <returns></returns>
	EmitterSphere ConvertParticleToEmitterSphere(const Particle& particle);

	// プレビュー用のparticle
	std::unique_ptr<GPUParticle> previewParticle_;

public:

#pragma region setter

	/// <summary>
	/// 値のセット
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="groupName"></param>
	/// <param name="key"></param>
	/// <param name="value"></param>
	template <typename T>
	void SetValue(const string& particleName, const string& key, const T& value);

	/// <summary>
	///	項目の追加
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="groupName"></param>
	/// <param name="key"></param>
	/// <param name="value"></param>
	template <typename T>
	void AddParam(const string& particleName, const string& key, const T& value);

#pragma endregion

#pragma region getter

	template <typename T>
	T GetValue(const string& particleName, const string& key) const;

#pragma endregion

private:
	// 全データ
	map<string, Particle> datas_;

	// json
	using json = nlohmann::json;

private:

	ParticleEditor() = default;
	~ParticleEditor() = default;
	ParticleEditor(const ParticleEditor&) = delete;
	ParticleEditor& operator=(const ParticleEditor&) = delete;
};

template<typename T>
inline void ParticleEditor::SetValue(const string& particleName, const string& key, const T& value)
{
	// グループの参照を取得
	Particle& particle = datas_[particleName];
	// 新しいパラメーターのデータを設定
	Param newParam{};
	newParam = value;
	// 設定したパラメーターをstd::mapに追加
	particle[key] = newParam;
}

template<typename T>
inline void ParticleEditor::AddParam(const string& particleName, const string& key, const T& value)
{
	Particle& particle = datas_[particleName];
	// 項目が未登録なら
	if (particle.find(key) == particle.end()) {
		SetValue(particleName, key, value);
	}
}

template<typename T>
inline T ParticleEditor::GetValue(const string& particleName, const string& key) const
{
	// 指定グループが存在しなかったら止める
	assert(datas_.find(particleName) != datas_.end());
	// グループの参照を取得
	const auto& itParticle = datas_.at(particleName);

	// particleに指定のキーが存在しなかったら止める
	assert(itParticle.find(key) != itParticle.end());

	// パラメーターの値の参照を取得
	const Param& itParam = itParticle.find(key)->second;

	return 	get<T>(itParam);

}

