#pragma once
#include <variant>
#include "engine/Math/Vector3.h"
#include <map>
#include <string>
#include "engine/Utility/ImGuiManager/ImGuiManager.h"

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
	using Item  = variant<int32_t, float, Vector3>;
	
	// グループ
	using Group = map<string, Item>;

	/// <summary>
	/// グループの作成
	/// </summary>
	/// <param name="groupName"></param>
	void CreateGroup(const string& groupName);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

#pragma region setter

	/// <summary>
	/// 値のセット
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="groupName"></param>
	/// <param name="key"></param>
	/// <param name="value"></param>
	template <typename T>
	void SetValue(const string& groupName, const string& key, T value);

#pragma endregion

private:
	// 全データ
	map<string, Group> datas_;

private:
	AdjustmentVariables() = default;
	~AdjustmentVariables() = default;
	AdjustmentVariables(const AdjustmentVariables&) = delete;
	const AdjustmentVariables& operator=(const AdjustmentVariables&) = delete;
};

template<typename T>
inline void AdjustmentVariables::SetValue(const string& groupName, const string& key, T value)
{
	// グループの参照を取得
	Group& group = datas_[groupName];
	// 新しい項目のデータを設定
	Item newItem{};
	newItem = value;
	// 設定した項目をstd::mapに追加
	group.items[key] = newItem;
}
