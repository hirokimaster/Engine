#pragma once

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

struct ParticleRange1d {
	float min;
	float max;
	float padding[2];
};

struct ParticleRange3d {
	Vector3 min;
	float padding1[1];
	Vector3 max;
	float padding2[1];
};

using std::variant;
using std::map;
using std::string;

class ParticleEditor {
public:

	// パラメーター
	using Param = variant<int32_t, float, Vector3, ParticleRange1d, ParticleRange3d>;

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
	if (!std::filesystem::exists(key)) {
		SetValue(particleName, key, value);
	}
}

template<typename T>
inline T ParticleEditor::GetValue(const string& particleName, const string& key) const
{
	// 指定グループが存在しなかったら止める
	assert(datas_.find(particleName) != datas_.end());
	// グループの参照を取得
	const auto& itGroup = datas_.at(particleName);

	// particleに指定のキーが存在しなかったら止める
	assert(itGroup.find(key) != itGroup.end());

	// パラメーターの値の参照を取得
	const Param& itParam = itGroup.find(key)->second;

	return 	get<T>(itParam);

}

