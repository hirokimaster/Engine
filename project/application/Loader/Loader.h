/**
* @file Loader.h
* @brief Blenderから出力したjsonファイルの読み込み.。それを基にオブジェクトを配置
* @author 仁平 琉乃
*/

#pragma once
#include <string>
#include <cassert>
#include <iostream>
#include <fstream>
#include "externals/Json/json.hpp"
#include "engine/Math/Vector3.h"
#include "engine/3d/Object3DPlacer/Object3DPlacer.h"
#include "engine/3d/Model/ModelManager.h"
#include <numbers>
#include "engine/Graphics/Effects/Particle/ParticleManager.h"
#include "application/GameObject/Obstacles/Laser.h"
#include "application/GameObject/Obstacles/Wall/Wall.h"

// レベルデータ
struct LevelData {
	// オブジェクト一個分のデータ
	struct ObjectData {
		std::string fileName;
		Vector3 translate;
		Vector3 rotate;
		Vector3 scale;
		std::vector<Vector3> controlPoint;
		uint32_t eventNum;
	};
	// オブジェクトのコンテナ
	std::vector<ObjectData>objects;
};

// 前方宣言
class ObjectManager;

class Loader {
public:

	/// <summary>
	/// ロードしたデータを記録
	/// </summary>
	void Record();

	/// <summary>
	/// objectを登録する
	/// </summary>
	/// <param name="ptr"></param>
	void ObjectRegister(ObjectManager* ptr);

#pragma region getter

	const std::map<std::string, std::vector<LevelData::ObjectData>>& GetObjectDatas() const {return objectDatas_;}

#pragma endregion


private:

	/// <summary>
	/// leveldataのロード
	/// </summary>
	/// <param name="fileName"></param>
	LevelData* Load(const std::string& fileName);

private:
	std::map<std::string, std::unique_ptr<Model>>  models_;
	std::map<std::string, std::vector<LevelData::ObjectData>> objectDatas_;
	UVTransform uvTransform_{
		{1.0f,2.5f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
	};
	LevelData* levelData_;
};