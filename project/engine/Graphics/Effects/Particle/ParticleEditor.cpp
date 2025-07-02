/**
* @file ParticleEditor.cpp
* @brief jsonからparamを読み込む
* @author 仁平 琉乃
*/

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

    static char newName[128] = "";
    static int selectedIndex = -1;
    int currentIndex = 0;

    // テクスチャ選択用
    static std::vector<std::string> textureFiles;
    static int selectedTextureIndex = 0;
    static std::string selectedTextureName;

    // テクスチャ一覧の初期化（初回のみ）
    static bool textureListInitialized = false;
    if (!textureListInitialized) {
        textureFiles.clear();
        std::filesystem::path texDir("resources/TempTexture/");
        if (std::filesystem::exists(texDir)) {
            for (const auto& entry : std::filesystem::directory_iterator(texDir)) {
                if (entry.is_regular_file()) {
                    std::string ext = entry.path().extension().string();
                    // 画像拡張子のみ
                    if (ext == ".png" || ext == ".jpg" || ext == ".dds" || ext == ".tga" || ext == ".bmp") {
                        std::string filename = entry.path().filename().string();
                        textureFiles.push_back(filename);
                        // ここでロード
                        std::string fullPath = "resources/TempTexture/" + filename;
                        TextureManager::Load(fullPath);
                    }
                }
            }
        }
        if (!textureFiles.empty()) {
            selectedTextureName = textureFiles[0];
        }
        textureListInitialized = true;
    }

    if (!ImGui::Begin("Particle Editor", nullptr, ImGuiWindowFlags_MenuBar)) {
        ImGui::End();
        return;
    }

    // 新規作成UI
    ImGui::InputText("New Particle Name", newName, sizeof(newName));
    ImGui::SameLine();
    if (ImGui::Button("Create")) {
        std::string nameStr = newName;
        if (!nameStr.empty() && datas_.find(nameStr) == datas_.end()) {
            CreateParticle(nameStr);
            selectedIndex = static_cast<int>(std::distance(datas_.begin(), datas_.find(nameStr)));
            newName[0] = '\0'; // 入力欄をクリア
        }
    }

    // テクスチャ選択UI
    if (!textureFiles.empty()) {
        std::vector<const char*> items;
        for (const auto& tex : textureFiles) items.push_back(tex.c_str());
        ImGui::Text("Texture:");
        if (ImGui::Combo("##TextureCombo", &selectedTextureIndex, items.data(), static_cast<int>(items.size()))) {
            selectedTextureName = textureFiles[selectedTextureIndex];
        }
    } else {
        ImGui::Text("No textures found in resources/TempTexture/");
    }

    // 既存パーティクルのリスト
    ImGui::Text("Select Particle:");
    if (ImGui::BeginListBox("##ParticleList", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing()))) {
        currentIndex = 0;
        for (auto it = datas_.begin(); it != datas_.end(); ++it, ++currentIndex) {
            const bool isSelected = (selectedIndex == currentIndex);
            if (ImGui::Selectable(it->first.c_str(), isSelected)) {
                selectedIndex = currentIndex;
            }
        }
        ImGui::EndListBox();
    }

    // 編集UI
    if (selectedIndex >= 0 && selectedIndex < static_cast<int>(datas_.size())) {
        auto it = datas_.begin();
        std::advance(it, selectedIndex);
        const std::string& particleName = it->first;
        Particle& particle = it->second;

        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu(particleName.c_str())) {
                for (auto& params : particle) {
                    const string& itemName = params.first;
                    Param& param = params.second;

                    if (holds_alternative<int32_t>(param)) {
                        int32_t* ptr = get_if<int32_t>(&param);
                        ImGui::DragInt(itemName.c_str(), ptr, 1.0f, 0, 100);
                    }
                    else if (holds_alternative<float>(param)) {
                        float* ptr = get_if<float>(&param);
                        ImGui::DragFloat(itemName.c_str(), ptr, 0.1f, -100.0f, 100.0f, "%.2f");
                    }
                    else if (holds_alternative<Vector3>(param)) {
                        Vector3* ptr = get_if<Vector3>(&param);
                        ImGui::DragFloat3(itemName.c_str(), reinterpret_cast<float*>(ptr), 0.1f, -100.0f, 100.0f, "%.2f");
                    }
                }

                ImGui::Text("\n");

                if (ImGui::Button("Save")) {
                    SaveFile(particleName);
                    string message = format("{}.json saved.", particleName);
                    MessageBoxA(nullptr, message.c_str(), "particleEditor", 0);
                }

                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        // プレビュー機能
        // プレビュー用パーティクルがなければ生成
        if (!previewParticle_) {
            previewParticle_ = std::make_unique<GPUParticle>();
            previewParticle_->SetModel("Player/plane.obj");
            previewParticle_->Initialize();
            previewParticle_->SetIsActive(true);
        }

        // ここで毎フレームパラメータをセット
        if (previewParticle_) {
            EmitterSphere param = ConvertParticleToEmitterSphere(particle);
            if (!selectedTextureName.empty()) {
                std::string texPath = "TempTexture/" + selectedTextureName;
                uint32_t texHandle = TextureManager::GetTexHandle(texPath);
                previewParticle_->SetTexHandle(texHandle);
            }
            previewParticle_->SetParticleParam(param);
            previewParticle_->Update();
        }
    }

    ImGui::End();

#endif
}

void ParticleEditor::Draw(const Camera& camera) {

    if (previewParticle_ && previewParticle_->GetIsActive()) {
        previewParticle_->Draw(camera);
    }
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

EmitterSphere ParticleEditor::ConvertParticleToEmitterSphere(const ParticleEditor::Particle& particle)
{
    EmitterSphere param{};

    // Vector3
    if (auto it = particle.find("translate"); it != particle.end() && std::holds_alternative<Vector3>(it->second))
        param.translate = std::get<Vector3>(it->second);

    if (auto it = particle.find("velocity"); it != particle.end() && std::holds_alternative<Vector3>(it->second))
        param.velocity = std::get<Vector3>(it->second);

    // float
    if (auto it = particle.find("radius"); it != particle.end() && std::holds_alternative<float>(it->second))
        param.radius = std::get<float>(it->second);

    if (auto it = particle.find("frequency"); it != particle.end() && std::holds_alternative<float>(it->second))
        param.frequency = std::get<float>(it->second);

    if (auto it = particle.find("frequencyTime"); it != particle.end() && std::holds_alternative<float>(it->second))
        param.frequencyTime = std::get<float>(it->second);

    // uint32_t
    if (auto it = particle.find("count"); it != particle.end()) {
        if (std::holds_alternative<int32_t>(it->second))
            param.count = static_cast<uint32_t>(std::get<int32_t>(it->second));
    }
    if (auto it = particle.find("emit"); it != particle.end()) {
        if (std::holds_alternative<int32_t>(it->second))
            param.emit = static_cast<uint32_t>(std::get<int32_t>(it->second));
    }

    // ParticleRange3d, ParticleRange1d
    auto getRange3d = [&](const std::string& minKey, const std::string& maxKey, ParticleRange3d& range) {
        if (auto it = particle.find(minKey); it != particle.end() && std::holds_alternative<Vector3>(it->second))
            range.min = std::get<Vector3>(it->second);
        if (auto it = particle.find(maxKey); it != particle.end() && std::holds_alternative<Vector3>(it->second))
            range.max = std::get<Vector3>(it->second);
        };
    auto getRange1d = [&](const std::string& minKey, const std::string& maxKey, ParticleRange1d& range) {
        if (auto it = particle.find(minKey); it != particle.end() && std::holds_alternative<float>(it->second))
            range.min = std::get<float>(it->second);
        if (auto it = particle.find(maxKey); it != particle.end() && std::holds_alternative<float>(it->second))
            range.max = std::get<float>(it->second);
        };

    getRange3d("rangeTranslate_min", "rangeTranslate_max", param.rangeTranslate);
    getRange3d("rangeScale_min", "rangeScale_max", param.rangeScale);
    getRange1d("rangeLifeTime_min", "rangeLifeTime_max", param.rangeLifeTime);
    getRange3d("rangeVelocity_min", "rangeVelocity_max", param.rangeVelocity);
    getRange1d("rangeCurrentTime_min", "rangeCurrentTime_max", param.rangeCurrentTime);
    getRange3d("rangeColor_min", "rangeColor_max", param.rangeColor);
    getRange1d("rangeAlpha_min", "rangeAlpha_max", param.rangeAlpha);

    return param;
}
