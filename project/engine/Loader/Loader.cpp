#include "Loader.h"

LevelData* Loader::Load(const std::string& fileName)
{
	// 連結してフルパスを得る
	const std::string fullpath = "resources/" + fileName + ".json";

	// ファイルストリーム
	std::ifstream file;

	// ファイルを開く
	file.open(fullpath);
	// ファイルオープン失敗をチェック
	if (file.fail()) {
		assert(0);
	}

	// JSON文字列から解凍したデータ
	nlohmann::json deserialized;

	// 解凍
	file >> deserialized;

	// 正しいレベルデータファイルかチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	// "name"を文字列として取得
	std::string name =
		deserialized["name"].get<std::string>();
	// 正しいレベルデータファイルかチェック
	assert(name.compare("scene") == 0);

	// レベルデータ格納用インスタンスを生成
	LevelData* levelData = new LevelData();

	// "objects"の全オブジェクトを走査
	for (nlohmann::json& object : deserialized["objects"]) {
		assert(object.contains("type"));
		// 種別を取得
		std::string type = object["type"].get<std::string>();
		// MESH												
		if (type.compare("MESH") == 0) {
			// 要素追加
			levelData->objects.emplace_back(LevelData::ObjectData{});
			// 今追加した要素の参照を得る
			LevelData::ObjectData& objectData = levelData->objects.back();

			if (object.contains("file_name")) {
				// ファイル名
				objectData.fileName = object["file_name"];
			}

			// トランスフォームのパラメータ読み込み
			nlohmann::json& transform = object["transform"];
			// 平行移動
			objectData.translate.x = (float)transform["translation"][0];
			objectData.translate.y = (float)transform["translation"][1];
			objectData.translate.z = (float)transform["translation"][2];
			// 回転角
			objectData.rotate.x = (float)transform["rotation"][0];
			objectData.rotate.y = (float)transform["rotation"][1];
			objectData.rotate.z = (float)transform["rotation"][2];
			// スケーリング
			objectData.scale.x = (float)transform["scaling"][0];
			objectData.scale.y = (float)transform["scaling"][1];
			objectData.scale.z = (float)transform["scaling"][2];
			// modelのロード
			ModelManager::GetInstance()->LoadObjModel("cube.obj");
		}

		// オブジェクト走査を再帰関数にまとめ、再帰関数で枝を走査する
		if (object.contains("children")) {

		}
	}

	return levelData;
}

void Loader::Arrangement(LevelData* levelData)
{
	// レベルデータからオブジェクトを生成、配置
	for (auto& objectData : levelData->objects) {

		// モデルを指定して3Dオブジェクトを生成
		std::unique_ptr<Object3DPlacer> newObject = std::make_unique<Object3DPlacer>();
		newObject->Initialize();
		newObject->SetModel("cube.obj");
		newObject->SetTexHandle(texHandle_);
		WorldTransform transform{};
		transform.Initialize();
		transform.translate = objectData.translate;
		transform.rotate = objectData.rotate;
		transform.scale = objectData.scale;
		transforms_.push_back(transform);
		objects_.push_back(std::move(newObject));
	}
}

void Loader::Draw(Camera& camera)
{
	for (auto& object : objects_) {
		for (auto& transform : transforms_) {
			transform.UpdateMatrix();
			object->Draw(transform, camera);
			ImGui::Begin("transform");
			ImGui::Text("%.2f %.2f %.2f", transform.translate.x, transform.translate.y, transform.translate.z);
			ImGui::End();
		}
	}

	
}

