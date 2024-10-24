/**
* @file ModelResources.h
* @brief モデルの読み込みをまとめる
* @author 仁平 琉乃
*/

#pragma once
#include "engine/ModelManager/ModelManager.h"

class ModelResources {
public:
	/// <summary>
	/// シングルトンインスタンス取得
	/// </summary>
	/// <returns></returns>
	static ModelResources* GetInstance();
	
	/// <summary>
	/// モデル読み込む場所
	/// </summary>
	void LoadModel();

private:
	ModelResources() = default;
	~ModelResources() = default;
	ModelResources(const ModelResources&) = delete;
	ModelResources& operator=(const ModelResources&) = delete;
};
#pragma once
