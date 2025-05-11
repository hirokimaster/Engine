/**
* @file Loader.cpp
* @brief Blenderから出力したjsonファイルの読み込み。それを基にオブジェクトを配置
* @author 仁平 琉乃
*/

#include "Loader.h"
#include "application/GameObject/Player/Player.h"
#include <application/GameObject/EnemyManager/Enemy/MoveEnemy/MoveEnemy.h>
#include "engine/3d/ObjectManager/ObjectManager.h"

LevelData* Loader::Load(const std::string& fileName)
{
	// 連結してフルパスを得る
	const std::string fullpath = "resources/LevelEditorObj/" + fileName + ".json";

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

			if (object.contains("event_id")) {
				// イベント番号
				objectData.eventNum = object["event_id"];
			}

			// トランスフォームのパラメータ読み込み
			nlohmann::json& transform = object["transform"];
			// 平行移動
			objectData.translate.x = (float)transform["translation"][0];
			objectData.translate.y = (float)transform["translation"][2];
			objectData.translate.z = (float)transform["translation"][1];
			// 回転角
			objectData.rotate.x = (float)transform["rotation"][0];

			objectData.rotate.y = ((float)transform["rotation"][2] - std::numbers::pi_v<float>);

			objectData.rotate.z = (float)transform["rotation"][1];
			// スケーリング
			objectData.scale.x = (float)transform["scaling"][0];
			objectData.scale.y = (float)transform["scaling"][2];
			objectData.scale.z = (float)transform["scaling"][1];

		}
		else if (type.compare("CURVE") == 0) {
			// 要素追加
			levelData->objects.emplace_back(LevelData::ObjectData{});
			// 今追加した要素の参照を得る
			LevelData::ObjectData& objectData = levelData->objects.back();

			if (object.contains("file_name")) {
				// ファイル名
				objectData.fileName = object["file_name"];
			}


			if (object.contains("event_id")) {
				// イベント番号
				objectData.eventNum = object["event_id"];
			}

			nlohmann::json& controlPoints = object["control_points"];

			if (controlPoints.is_array()) {
				for (auto it = controlPoints.rbegin(); it != controlPoints.rend(); ++it) {  // 逆順
					auto& point = *it;

					if (point.contains("x") && point.contains("y") && point.contains("z")) {
						float x = point["x"];
						float y = point["z"];
						float z = point["y"];

						Vector3 newControlPoint{};
						newControlPoint.x = x;
						newControlPoint.y = y;
						newControlPoint.z = z;
						objectData.controlPoint.push_back(newControlPoint);
					}

				}
			}
		}
	}

	return levelData;
}

void Loader::Record()
{
	// 使うテクスチャ
	TextureManager::Load("resources/TempTexture/noise0.png");
	TextureManager::Load("resources/Stage/road.png");
	TextureManager::Load("resources/Stage/floor.png");
	TextureManager::Load("resources/TempTexture/mount.jpg");
	TextureManager::Load("resources/TempTexture/uvChecker.png");

	levelData_ = Load("level2");

	// jsonファイルから読み込んだ情報を保存する
	for (auto& objectData : levelData_->objects) {
		// 動く敵
		if (objectData.fileName == "moveEnemy") {
			objectDatas_[objectData.fileName].push_back(objectData);
		}
		// 固定の敵
		else if (objectData.fileName == "fixedEnemy") {
			objectDatas_[objectData.fileName].push_back(objectData);
		}
		// レーザー
		else if (objectData.fileName == "laser") {
			objectDatas_[objectData.fileName].push_back(objectData);
		}
		// 道
		else if (objectData.fileName == "roads") {
			objectDatas_[objectData.fileName].push_back(objectData);
		}
		// 地面
		else if (objectData.fileName == "grounds") {
			objectDatas_[objectData.fileName].push_back(objectData);
		}
		// 壁
		else if (objectData.fileName == "wall") {
			objectDatas_[objectData.fileName].push_back(objectData);
		}
		// 床
		else if (objectData.fileName == "floor") {
			objectDatas_[objectData.fileName].push_back(objectData);
		}
		// 山
		else if (objectData.fileName == "mount") {
			objectDatas_[objectData.fileName].push_back(objectData);
		}
		// イベントトリガー
		else if (objectData.fileName == "EventTrigger") {
			objectDatas_[objectData.fileName].push_back(objectData);
		}
	}
}

void Loader::ObjectRegister()
{
	// レーザー
	auto it = GetObjectDatas().find("laser");
	if (it != GetObjectDatas().end()) {
		for (auto& objectData : it->second) {
			std::unique_ptr<Laser> object = std::make_unique<Laser>();
			object->Initialize();
			object->SetPosition(objectData.translate);
			object->SetScale(objectData.scale);
		}
	}
	// 道
	auto it2 = GetObjectDatas().find("roads");
	if (it2 != GetObjectDatas().end()) {
		for (auto& objectData : it2->second) {
			std::unique_ptr<BaseInstancingObject> object = std::make_unique<BaseInstancingObject>();
			object->Initialize("LevelEditorObj/roads.obj", "Stage/road.png");
			object->SetPosition(objectData.translate);
			object->SetRotate(objectData.rotate);
			object->SetScale(objectData.scale);
		}
	}
	// 地面
	auto it3 = GetObjectDatas().find("grounds");
	if (it3 != GetObjectDatas().end()) {
		for (auto& objectData : it3->second) {
			std::unique_ptr<BaseInstancingObject> object = std::make_unique<BaseInstancingObject>();
			object->Initialize("LevelEditorObj/grounds.obj", "TempTexture/mount.jpg");
			object->SetPosition(objectData.translate);
			object->SetRotate(objectData.rotate);
			object->SetScale(objectData.scale);
		}
	}
	// 壁
	auto it4 = GetObjectDatas().find("wall");
	if (it4 != GetObjectDatas().end()) {
		for (auto& objectData : it4->second) {
			std::unique_ptr<Wall> object = std::make_unique<Wall>();
			object->Initialize();
			object->SetPosition(objectData.translate);
			object->SetScale(objectData.scale);
		}
	}

	// 床
	auto it5 = GetObjectDatas().find("floor");
	if (it5 != GetObjectDatas().end()) {
		for (auto& objectData : it5->second) {
			std::unique_ptr<BaseInstancingObject> object = std::make_unique<BaseInstancingObject>();
			object->Initialize("LevelEditorObj/floor.obj", "Stage/floor.png");
			object->SetPosition(objectData.translate);
			object->SetRotate(objectData.rotate);
			object->SetScale(objectData.scale);
		}
	}

	// 山
	auto it6 = GetObjectDatas().find("mount");
	if (it6 != GetObjectDatas().end()) {
		for (auto& objectData : it6->second) {
			std::unique_ptr<BaseInstancingObject> object = std::make_unique<BaseInstancingObject>();
			object->Initialize("LevelEditorObj/part.obj", "TempTexture/mount.jpg");
			object->SetPosition(objectData.translate);
			object->SetRotate(objectData.rotate);
			object->SetScale(objectData.scale);
		}
	}
}
