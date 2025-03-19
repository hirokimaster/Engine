/**
* @file Loader.cpp
* @brief Blenderから出力したjsonファイルの読み込み。それを基にオブジェクトを配置
* @author 仁平 琉乃
*/

#include "Loader.h"
#include "application/GameObject/Player/Player.h"

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

	// カメラの初期化
	camera_.Initialize();

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

			// modelのロード
			if (objectData.fileName != "eventTrigger" && objectData.fileName != "floor" && objectData.fileName != "wall") {
				ModelManager::GetInstance()->LoadObjModel("LevelEditorObj/" + objectData.fileName + ".obj");	
			}

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

						ModelManager::GetInstance()->LoadObjModel("LevelEditorObj/" + objectData.fileName + ".obj");
					}

					if (object.contains("children")) {
						// 再帰処理を追加（未実装部分）
					}
				}
			}
		}
	}

	return levelData;
}

void Loader::Arrangement()
{
	// 使うテクスチャ
	TextureManager::Load("resources/TempTexture/noise0.png");
	TextureManager::Load("resources/Stage/road.png");
	TextureManager::Load("resources/TempTexture/mount.jpg");
	TextureManager::Load("resources/TempTexture/uvChecker.png");

	levelData_ = Load("level2");

	// レベルデータからオブジェクトを生成、配置
	for (auto& objectData : levelData_->objects) {

		// モデルを指定して3Dオブジェクトを生成
		if (objectData.fileName == "enemy") {
			std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
			newEnemy->Initialize(TextureManager::GetTexHandle("TempTexture/noise0.png"));
			newEnemy->SetPlayer(player_);
			newEnemy->SetPosition(objectData.controlPoint[0]); // 初期位置は移動ルートの最初の制御点
			newEnemy->SetEventNum(objectData.eventNum);
			for (auto& point : objectData.controlPoint) {
				newEnemy->SetMoveControlPoints(point);
			}
			enemys_.push_back(std::move(newEnemy));
		}
		else if (objectData.fileName == "laser") {
			std::unique_ptr<Laser> newLaser = std::make_unique<Laser>();
			newLaser->Initialize();
			newLaser->SetPosition(objectData.translate);
			lasers_.push_back(std::move(newLaser));
		}
		else if (objectData.fileName == "roads") {
			std::unique_ptr<Object3DPlacer> newObject = std::make_unique<Object3DPlacer>();
			newObject->Initialize();
			newObject->SetModel("LevelEditorObj/grounds.obj");
			newObject->SetTexHandle(TextureManager::GetTexHandle("Stage/road.png"));
			newObject->SetPosition(objectData.translate);
			newObject->SetRotate(objectData.rotate);
			newObject->SetScale(objectData.scale);
			newObject->SetUVTransform(uvTransform_);
			objects_.push_back(std::move(newObject));
		}
		else if (objectData.fileName == "grounds") {
			std::unique_ptr<Object3DPlacer> newObject = std::make_unique<Object3DPlacer>();
			newObject->Initialize();
			newObject->SetModel("LevelEditorObj/" + objectData.fileName + ".obj");
			newObject->SetTexHandle(TextureManager::GetTexHandle("TempTexture/mount.jpg"));
			newObject->SetPosition(objectData.translate);
			newObject->SetRotate(objectData.rotate);
			newObject->SetScale(objectData.scale);
			objects_.push_back(std::move(newObject));
		}
		else if (objectData.fileName == "mounts2") {
			std::unique_ptr<Object3DPlacer> newObject = std::make_unique<Object3DPlacer>();
			newObject->Initialize();
			newObject->SetModel("LevelEditorObj/" + objectData.fileName + ".obj");
			newObject->SetTexHandle(TextureManager::GetTexHandle("TempTexture/mount.jpg"));
			newObject->SetPosition(objectData.translate);
			newObject->SetRotate(objectData.rotate);
			newObject->SetScale(objectData.scale);
			objects_.push_back(std::move(newObject));
		}
		/*else if (objectData.fileName == "floor") {
			std::unique_ptr<Object3DPlacer> newObject = std::make_unique<Object3DPlacer>();
			newObject->Initialize();
			newObject->SetModel("LevelEditorObj/plane.obj");
			newObject->SetTexHandle(TextureManager::GetTexHandle("TempTexture/uvChecker.png"));
			newObject->SetPosition(objectData.translate);
			newObject->SetRotate(objectData.rotate);
			newObject->SetScale(objectData.scale);
			objects_.push_back(std::move(newObject));
		}*/
		else if (objectData.fileName == "wall") {
			std::unique_ptr<Object3DPlacer> newObject = std::make_unique<Object3DPlacer>();
			newObject->Initialize();
			newObject->SetModel("Player/cube.obj");
			newObject->SetTexHandle(TextureManager::GetTexHandle("TempTexture/uvChecker.png"));
			newObject->SetPosition(objectData.translate);
			newObject->SetRotate(objectData.rotate);
			newObject->SetScale(objectData.scale);
			objects_.push_back(std::move(newObject));
		}
		
	}

	for (auto& objectData : levelData_->objects) {
		if (objectData.fileName == "eventTrigger") {
			for (auto& enemy : enemys_) {
				if (enemy->GetEventNum() == objectData.eventNum) {
					enemy->SetEventTrigger(objectData.translate);
				}
			}
		}
	}
}

void Loader::Update()
{
	// objectの更新
	for (auto& object : objects_) {
		object->Update();
		object->SetUVTransform(uvTransform_);
	}

	// enemyの更新
	for (auto& enemy : enemys_) {
		enemy->Update();
	}

	// laserの更新
	for (auto& laser : lasers_) {
		laser->Update();
	}

	// デスフラグが立ったら要素を削除
	enemys_.remove_if([](std::unique_ptr<Enemy>& enemy) {
		if (enemy->GetIsDead()) {
			return true;
		}
		return false;
		});
}

void Loader::Draw(const Camera& camera)
{
	for (auto& object : objects_) {
		object->Draw(camera);
	}

	for (auto& enemy : enemys_) {
		enemy->Draw(camera);
	}

	for (auto& laser : lasers_) {
		laser->Draw(camera);
	}
}

void Loader::UpdateCamera()
{
	camera_.UpdateMatrix();
}

