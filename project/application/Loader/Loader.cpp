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
			ModelManager::GetInstance()->LoadObjModel("LevelEditorObj/" + objectData.fileName + ".obj");

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

			nlohmann::json& controlPoints = object["control_points"];

			if (controlPoints.is_array()) {
				// 制御点を逆順で読み込む
				int controlPointIndex = 0;
				for (auto& point : controlPoints) {

					if (point.contains("x") && point.contains("y") && point.contains("z")) {
						// 各座標を設定
						float x = point["x"];
						float y = point["y"];
						float z = point["z"];

						Vector3 newControlPoint{};
						newControlPoint.x = x;
						newControlPoint.y = y;
						newControlPoint.z = z;
						objectData.controlPoint.push_back(newControlPoint);

						// インデックスを更新
						controlPointIndex++;

						// modelのロード
						ModelManager::GetInstance()->LoadObjModel("LevelEditorObj/" + objectData.fileName + ".obj");

					}

					// オブジェクト走査を再帰関数にまとめ、再帰関数で枝を走査する
					if (object.contains("children")) {

					}
				}

				return levelData;
			}
		}
	}
}

void Loader::Arrangement()
{
	// 使うテクスチャ
	TextureManager::Load("resources/TempTexture/noise0.png");
	TextureManager::Load("resources/Stage/road.png");
	TextureManager::Load("resources/TempTexture/mount.jpg");

	levelData_ = Load("level2");

	// レベルデータからオブジェクトを生成、配置
	for (auto& objectData : levelData_->objects) {

		// モデルを指定して3Dオブジェクトを生成
		if (objectData.fileName == "enemyMoveRoute") {
			std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
			newEnemy->Initialize(TextureManager::GetTexHandle("TempTexture/noise0.png"));
			newEnemy->SetPlayer(player_);
			newEnemy->SetPosition(objectData.controlPoint[0]); // 初期位置は移動ルートの最初の制御点
			for (auto& point : objectData.controlPoint) {
				newEnemy->SetMoveControlPoints(point);
			}
			enemys_.push_back(std::move(newEnemy));
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
		else if (objectData.fileName == "laser") {
			std::unique_ptr<Laser> newLaser = std::make_unique<Laser>();
			newLaser->Initialize();
			newLaser->SetPosition(objectData.translate);
			lasers_.push_back(std::move(newLaser));
		}
		else if (objectData.fileName == "rail") {
	    // デバックの時だけ線描画する
        #ifdef _DEBUG
			for (uint32_t i = 0; i < 100; ++i) {
				std::unique_ptr<Line> line = std::make_unique<Line>();
				line->Initialize({ 0,0,0 }, { 0,0,0 });
				railLine_.push_back(std::move(line));
			}
        #endif
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

void Loader::Draw(Camera& camera)
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

