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
#include "engine/Math/Vector2.h"

using std::variant;
using std::map;
using std::string;

class AdjustmentVariables {
public:

	/// <summary>
	/// インスタンス取得
	/// </summary>
	/// <returns></returns>
	static AdjustmentVariables* GetInstance();

	// 項目
	using Item = variant<int32_t, float, Vector2, Vector3>;

	// グループ
	using Group = map<string, Item>;

	// グローバル変数の保存先ファイルパス
	const string kDirectoryPath = "resources/AdjustmentVariables/";

	/// <summary>
	/// グループの作成
	/// </summary>
	/// <param name="groupName"></param>
	void CreateGroup(const string& groupName);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// ファイルに書き出し
	/// </summary>
	/// <param name="groupName"></param>
	void SaveFile(const std::string& groupName);

	/// <summary>
	/// ディレクトリの全ファイル読み込み
	/// </summary>
	void LoadFiles();

	/// <summary>
	/// ファイルから読み込む
	/// </summary>
	/// <param name="groupName"></param>
	void LoadFile(const string& groupName);

#pragma region setter

	/// <summary>
	/// 値のセット
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="groupName"></param>
	/// <param name="key"></param>
	/// <param name="value"></param>
	template <typename T>
	void SetValue(const string& groupName, const string& key, const T& value);

	/// <summary>
	///	項目の追加
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="groupName"></param>
	/// <param name="key"></param>
	/// <param name="value"></param>
	template <typename T>
	void AddItem(const string& groupName, const string& key, const T& value);

#pragma endregion

#pragma region getter

	template <typename T>
	T GetValue(const string& groupName, const string& key) const;

#pragma endregion

private:
	// 全データ
	map<string, Group> datas_;

	// json
	using json = nlohmann::json;

private:
	AdjustmentVariables() = default;
	~AdjustmentVariables() = default;
	AdjustmentVariables(const AdjustmentVariables&) = delete;
	const AdjustmentVariables& operator=(const AdjustmentVariables&) = delete;
};

template<typename T>
inline void AdjustmentVariables::SetValue(const string& groupName, const string& key, const T& value)
{
	// グループの参照を取得
	Group& group = datas_[groupName];
	// 新しい項目のデータを設定
	Item newItem{};
	newItem = value;
	// 設定した項目をstd::mapに追加
	group[key] = newItem;
}

template<typename T>
inline void AdjustmentVariables::AddItem(const string& groupName, const string& key, const T& value)
{
	Group& group = datas_[groupName];
	// 項目が未登録なら
	if (group.find(key) == group.end()) {
		SetValue(groupName, key, value);
	}
}

template<typename T>
inline T AdjustmentVariables::GetValue(const string& groupName, const string& key) const
{
	// 指定グループが存在しなかったら止める
	assert(datas_.find(groupName) != datas_.end());
	// グループの参照を取得
	const auto& itGroup = datas_.at(groupName);

	// 指定グループに指定のキーが存在しなかったら止める
	assert(itGroup.find(key) != itGroup.end());

	// アイテムの値の参照を取得
	const Item& itItem = itGroup.find(key)->second;

	return 	get<T>(itItem);

}
