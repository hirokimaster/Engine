#include "ParticleEditor.h"

ParticleEditor* ParticleEditor::GetInstance()
{
	static ParticleEditor instance;
	return &instance;
}

void ParticleEditor::CreateParticle(const string& particleName)
{
	// 指定名のオブジェクトが無ければ追加する
	datas_[particleName];
}

void ParticleEditor::Update()
{
#ifdef _DEBUG

	if (!ImGui::Begin("Particle Editor", nullptr, ImGuiWindowFlags_MenuBar)) {
		ImGui::End();
		return;
	}

	if (!ImGui::BeginMenuBar()) {
		return;
	}
	// 各particleについて
	for (auto& particles : datas_) {
		// particleの名を取得
		const string& particleName = particles.first;
		// particleの参照を取得
		Particle& particle = particles.second;

		if (!ImGui::BeginMenu(particleName.c_str())) {
			continue;
		}

		// 各パラメーターについて
		for (auto& params : particle) {
			// パラメーター名を取得
			const string& itemName = params.first;
			// パラメーターの参照を取得
			Param& param = params.second;

			// int32_t型
			if (holds_alternative<int32_t>(param)) {
				int32_t* ptr = get_if<int32_t>(&param);
				ImGui::SliderInt(itemName.c_str(), ptr, 0, 100);
			}
			// uint32_t型
			else if (holds_alternative<uint32_t>(param)) {
				uint32_t* ptr = get_if<uint32_t>(&param);
				int sliderValue = static_cast<int>(*ptr);
				ImGui::SliderInt(itemName.c_str(), &sliderValue, 0, 100);
				*ptr = static_cast<uint32_t>(sliderValue);
			}
			// float型
			else if (holds_alternative<float>(param)) {
				float* ptr = get_if<float>(&param);
				ImGui::SliderFloat(itemName.c_str(), ptr, 0.0f, 100.0f);
			}
			// Vector3型
			else if (holds_alternative<Vector3>(param)) {
				Vector3* ptr = get_if<Vector3>(&param);
				ImGui::SliderFloat3(itemName.c_str(), reinterpret_cast<float*>(ptr), -10.0f, 10.0f);
			}
		}

		// 改行
		ImGui::Text("\n");

		if (ImGui::Button("Save")) {
			SaveFile(particleName);
			string message = format("{}.json saved.", particleName);
			MessageBoxA(nullptr, message.c_str(), "particleEditor", 0);
		}

		ImGui::EndMenu();
	}

	ImGui::EndMenuBar();
	ImGui::End();

#endif
}

void ParticleEditor::SaveFile(const std::string& particleName)
{
	// グループを検索
	map<string, Particle>::iterator itParticle = datas_.find(particleName);
	// 未登録チェック
	assert(itParticle != datas_.end());

	json root;

	root = json::object();

	// jsonオブジェクト登録
	root[particleName] = json::object();

	for (auto& params : itParticle->second) {
		// パラメーター名を取得
		const string& paramName = params.first;
		Param& param = params.second;

		// int32_t型
		if (holds_alternative<int32_t>(param)) {
			root[particleName][paramName] = get<int32_t>(param);
		}
		// uint32_t型
		else if (holds_alternative<uint32_t>(param)) {
			root[particleName][paramName] = get<uint32_t>(param);
		}
		// float型
		else if (holds_alternative<float>(param)) {
			root[particleName][paramName] = get<float>(param);
		}
		// Vector3型
		else if (holds_alternative<Vector3>(param)) {
			Vector3 value = get<Vector3>(param);
			root[particleName][paramName] = json::array({ value.x,value.y,value.z });
		}

	}

	// ディレクトリが無ければ作成する
	std::filesystem::path dir(kDirectoryPath);
	if (!std::filesystem::exists(dir)) {
		std::filesystem::create_directory(dir);
	}

	// 書き込むJSONファイルのフルパスを合成する
	string filePath = kDirectoryPath + particleName + ".json";
	// 書き込み用ファイルストリーム
	std::ofstream ofs;
	// ファイルを書き込みように開く
	ofs.open(filePath);

	// ファイルオープン失敗？
	if (ofs.fail()) {
		string message = "Failed open data file for write.";
		MessageBoxA(nullptr, message.c_str(), "ParticleEditor", 0);
		assert(0);
		return;
	}

	// ファイルにjson文字列を書き込む(インデント幅4）
	ofs << std::setw(4) << root << std::endl;
	// ファイルを閉じる
	ofs.close();

}

void ParticleEditor::LoadFiles()
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

void ParticleEditor::LoadFile(const string& particleName)
{
	// 読み込むJSONファイルのフルパスを合成する
	string filePath = kDirectoryPath + particleName + ".json";
	// 読み込み用ファイルストリーム
	std::ifstream ifs;
	// ファイルを読み込み用に開く
	ifs.open(filePath);

	// ファイルオープン失敗？
	if (ifs.fail()) {
		string message = "Failed open data file for write.";
		MessageBoxA(nullptr, message.c_str(), "ParticleEditor", 0);
		assert(0);
		return;
	}

	json root;

	// json文字列からjsonのデータ構造に展開
	ifs >> root;
	// ファイルを閉じる
	ifs.close();

	// particle検索
	json::iterator itParticle = root.find(particleName);
	// 未登録チェック
	assert(itParticle != root.end());

	// 各パラメータについて
	for (json::iterator itParam = itParticle->begin(); itParam != itParticle->end(); ++itParam) {
		// パラメータ名を取得
		const string& paramName = itParam.key();

		// int32_t型
		if (itParam->is_number_integer()) {
			int32_t value = itParam->get<int32_t>();
			SetValue(particleName, paramName, value);
		}
		// uint32_t型
		else if (itParam->is_number_unsigned()) {
			uint32_t value = itParam->get<uint32_t>();
			SetValue(particleName, paramName, value);
		}
		// float
		else if (itParam->is_number_float()) {
			double value = itParam->get<double>();
			SetValue(particleName, paramName, static_cast<float>(value));
		}
		// Vector3
		else if (itParam->is_array() && itParam->size() == 3) {
			Vector3 value = { itParam->at(0), itParam->at(1), itParam->at(2) };
			SetValue(particleName, paramName, value);
		}
	}
}
