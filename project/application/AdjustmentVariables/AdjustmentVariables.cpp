#include "AdjustmentVariables.h"

AdjustmentVariables* AdjustmentVariables::GetInstance()
{
	static AdjustmentVariables instance;
	return &instance;
}

void AdjustmentVariables::CreateGroup(const string& groupName)
{
	// 指定名のオブジェクトが無ければ追加する
	datas_[groupName];
}

void AdjustmentVariables::Update()
{
#ifdef _DEBUG

	if (!ImGui::Begin("Adjustment Variables", nullptr, ImGuiWindowFlags_MenuBar)) {
		ImGui::End();
		return;
	}

	if (!ImGui::BeginMenuBar()) {
		return;
	}
	// 各グループについて
	for (auto& groups : datas_) {
		// グループ名を取得
		const string& groupName = groups.first;
		// グループの参照を取得
		Group& group = groups.second;

		if (!ImGui::BeginMenu(groupName.c_str())) {
			continue;
		}

		// 各項目について
		for (auto& items : group) {
			// 項目名を取得
			const string& itemName = items.first;
			// 項目の参照を取得
			Item& item = items.second;

			// int32_t型
			if (holds_alternative<int32_t>(item)) {
				int32_t* ptr = get_if<int32_t>(&item);
				ImGui::DragInt(itemName.c_str(), ptr, 0, 1000);
			}
			// float型
			else if (holds_alternative<float>(item)) {
				float* ptr = get_if<float>(&item);
				ImGui::DragFloat(itemName.c_str(), ptr, 0.0f, 1000.0f);
			}
			// Vector2型
			else if (holds_alternative<Vector2>(item)) {
				Vector2* ptr = get_if<Vector2>(&item);
				ImGui::DragFloat2(itemName.c_str(), reinterpret_cast<float*>(ptr), 0.0f, 1280.0f);
			}
			// Vector3型
			else if (holds_alternative<Vector3>(item)) {
				Vector3* ptr = get_if<Vector3>(&item);
				ImGui::DragFloat3(itemName.c_str(), reinterpret_cast<float*>(ptr), -1000.0f, 1000.0f);
			}

		}

		// 改行
		ImGui::Text("\n");

		if (ImGui::Button("Save")) {
			SaveFile(groupName);
			string message = format("{}.json saved.", groupName);
			MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
		}

		ImGui::EndMenu();
	}

	ImGui::EndMenuBar();
	ImGui::End();

#endif
}

void AdjustmentVariables::SaveFile(const std::string& groupName)
{
	// グループを検索
	map<string, Group>::iterator itGroup = datas_.find(groupName);
	// 未登録チェック
	assert(itGroup != datas_.end());

	json root;

	root = json::object();

	// jsonオブジェクト登録
	root[groupName] = json::object();

	for (auto& items : itGroup->second) {
		// 項目名を取得
		const string& itemName = items.first;
		Item& item = items.second;

		// int32_t型
		if (holds_alternative<int32_t>(item)) {
			root[groupName][itemName] = get<int32_t>(item);
		}
		// float型
		else if (holds_alternative<float>(item)) {
			root[groupName][itemName] = get<float>(item);
		}
		// Vector2型
		else if (holds_alternative<Vector2>(item)) {
			Vector2 value = get<Vector2>(item);
			root[groupName][itemName] = json::array({ value.x, value.y });
		}
		// Vector3型
		else if (holds_alternative<Vector3>(item)) {
			Vector3 value = get<Vector3>(item);
			root[groupName][itemName] = json::array({ value.x,value.y,value.z });
		}

	}

	// ディレクトリが無ければ作成する
	std::filesystem::path dir(kDirectoryPath);
	if (!std::filesystem::exists(dir)) {
		std::filesystem::create_directory(dir);
	}

	// 書き込むJSONファイルのフルパスを合成する
	string filePath = kDirectoryPath + groupName + ".json";
	// 書き込み用ファイルストリーム
	std::ofstream ofs;
	// ファイルを書き込みように開く
	ofs.open(filePath);

	// ファイルオープン失敗？
	if (ofs.fail()) {
		string message = "Failed open data file for write.";
		MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
		assert(0);
		return;
	}

	// ファイルにjson文字列を書き込む(インデント幅4）
	ofs << std::setw(4) << root << std::endl;
	// ファイルを閉じる
	ofs.close();

}

void AdjustmentVariables::LoadFiles()
{
	// 保存先ディレクトリのパス
	std::filesystem::path dir(kDirectoryPath);
	// ディレクトリが無ければスキップする
	if (!std::filesystem::exists(dir)) {
		return;
	}

	std::filesystem::directory_iterator dir_it(dir);
	for (const std::filesystem::directory_entry& entry : dir_it) {
		// ファイルパスを取得
		const std::filesystem::path& filePath = entry.path();
		// ファイル拡張子を取得
		string extension = filePath.extension().string();
		// .jsonファイル以外はスキップ
		if (extension.compare(".json") != 0) {
			continue;
		}

		// ファイル読み込み
		LoadFile(filePath.stem().string());
	}
}

void AdjustmentVariables::LoadFile(const string& groupName)
{
	// 読み込むJSONファイルのフルパスを合成する
	string filePath = kDirectoryPath + groupName + ".json";
	// 読み込み用ファイルストリーム
	std::ifstream ifs;
	// ファイルを読み込み用に開く
	ifs.open(filePath);

	// ファイルオープン失敗？
	if (ifs.fail()) {
		string message = "Failed open data file for write.";
		MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
		assert(0);
		return;
	}

	json root;

	// json文字列からjsonのデータ構造に展開
	ifs >> root;
	// ファイルを閉じる
	ifs.close();

	// グループ検索
	json::iterator itGroup = root.find(groupName);
	// 未登録チェック
	assert(itGroup != root.end());

	// 各アイテムについて
	for (json::iterator itItem = itGroup->begin(); itItem != itGroup->end(); ++itItem) {
		// アイテム名を取得
		const string& itemName = itItem.key();

		// int32_t型
		if (itItem->is_number_integer()) {
			int32_t value = itItem->get<int32_t>();
			SetValue(groupName, itemName, value);
		}
		// float
		else if (itItem->is_number_float()) {
			double value = itItem->get<double>();
			SetValue(groupName, itemName, static_cast<float>(value));
		}
		else if (itItem->is_array() && itItem->size() == 2) {
			Vector2 value = { itItem->at(0), itItem->at(1) };
			SetValue(groupName, itemName, value);
		}
		// Vector3
		else if (itItem->is_array() && itItem->size() == 3) {
			Vector3 value = { itItem->at(0), itItem->at(1), itItem->at(2) };
			SetValue(groupName, itemName, value);
		}
	}
}
