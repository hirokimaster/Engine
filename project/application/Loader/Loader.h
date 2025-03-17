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
#include "application/GameObject/Enemy/Enemy.h"
#include "engine/Graphics/Effects/Particle/ParticleManager.h"
#include "application/GameObject/Gimmick/Laser.h"
#include "engine/3d/Line/Line.h"
#include "application/GameObject/Rail/Rail.h"

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

class Player;

class Loader {
public:

	/// <summary>
	/// ロードしたデータを基に配置
	/// </summary>
	void Arrangement();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const Camera& camera);

	/// <summary>
	/// カメラの更新
	/// </summary>
	void UpdateCamera();

#pragma region getter

	const Camera& GetCamera() { return camera_; }

	const std::list<std::unique_ptr<Enemy>>& GetEnemys() const{ return enemys_; }

	const std::list<std::unique_ptr<Laser>>& GetLasers() const { return lasers_; }

#pragma endregion

#pragma region setter

	void SetPlayer(Player* player) { player_ = player; }

	void SetTexHandle(uint32_t texHandle) { texHandle_ = texHandle; }

#pragma endregion

private:

	/// <summary>
	/// leveldataのロード
	/// </summary>
	/// <param name="fileName"></param>
	LevelData* Load(const std::string& fileName);

private:
	std::map<std::string, std::unique_ptr<Model>>  models_;
	std::vector<std::unique_ptr<Object3DPlacer>> objects_;
	std::list<std::unique_ptr<Enemy>> enemys_;
	std::list<std::unique_ptr<Laser>> lasers_;
	uint32_t texHandle_ = 0;
	Camera camera_{};
	Player* player_ = nullptr;
	UVTransform uvTransform_{
		{1.0f,2.5f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
	};
	LevelData* levelData_;
	std::vector<std::unique_ptr<Line>> railLine_;
};