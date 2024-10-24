/**
* @file Skydome.h
* @brief 天球生成
* @author 仁平 琉乃
*/

#pragma once
#include <memory>
#include "engine/Transform/WorldTransform.h"
#include "engine/Object3DPlacer/Object3DPlacer.h"
#include "engine/Camera/Camera.h"

class Skydome
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

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	void Draw(Camera& camera);

private:
	
	WorldTransform worldTransform_;
	std::unique_ptr<Object3DPlacer> object_;
	uint32_t texHandle_ = 0;
	Material material_{};
};