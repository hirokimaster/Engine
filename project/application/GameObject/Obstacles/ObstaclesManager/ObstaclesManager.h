#pragma once
#include "application/GameObject/Obstacles/Laser/Laser.h"
#include "application/GameObject/Obstacles/Wall/Wall.h"
#include "application/Loader/Loader.h"

class ObstaclesManager
{
public:

	/// <summary>
	 /// 初期化
	 /// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

#pragma region getter

	const std::list<std::unique_ptr<BaseInstancingObject>>& GetObstacles()const { return obstacles_; }

#pragma endregion

#pragma region setter

	void SetLoader(Loader* ptr) { loader_ = ptr; }

#pragma endregion

private:
	// objectのリスト
	std::list<std::unique_ptr<BaseInstancingObject>> obstacles_;
	// loader
	Loader* loader_ = nullptr;

};

