#pragma once
#include <string>
#include <cassert>
#include <iostream>
#include <fstream>
#include "externals/Json/json.hpp"
#include "engine/Math/Vector3.h"
#include "engine/Model/Model.h"
#include "engine/Object3DPlacer/Object3DPlacer.h"
#include "engine/ModelManager/ModelManager.h"
#include <numbers>

// レベルデータ
struct LevelData {
	// オブジェクト一個分のデータ
	struct ObjectData {
		std::string fileName;
		Vector3 translate;
		Vector3 rotate;
		Vector3 scale;
	};
	// オブジェクトのコンテナ
	std::vector<ObjectData>objects;
};

class Loader {
public:
	/// <summary>
	/// leveldataのロード
	/// </summary>
	/// <param name="fileName"></param>
	LevelData* Load(const std::string& fileName);

	/// <summary>
	/// ロードしたデータを基に配置
	/// </summary>
	void Arrangement(LevelData* levelData);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(Camera& camera);

	void SetTexHandle(uint32_t texHandle) { texHandle_ = texHandle; }

	void UpdateCamera();

#pragma region getter

	const Camera& GetCamera() { return camera_; }

#pragma endregion

private:
	std::map<std::string, std::unique_ptr<Model>>  models_;
	std::vector<std::unique_ptr<Object3DPlacer>> objects_;
	uint32_t texHandle_ = 0;
	Camera camera_{};
};