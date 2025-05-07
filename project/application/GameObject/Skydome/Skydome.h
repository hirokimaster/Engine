/**
* @file Skydome.h
* @brief 天球生成
* @author 仁平 琉乃
*/

#pragma once
#include <memory>
#include "engine/Graphics/Transform/WorldTransform.h"
#include "engine/3d/Object3dPlacer/Object3dPlacer.h"
#include "engine/Graphics/Camera/Camera.h"
#include "engine/Graphics/TextureManager/TextureManager.h"

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
	void Draw(const Camera& camera);

private:
	std::unique_ptr<Object3dPlacer> object_;
	Material material_{};
	UVTransform uvTransform_{};
};