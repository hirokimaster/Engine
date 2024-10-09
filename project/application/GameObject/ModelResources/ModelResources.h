#pragma once
#include "engine/ModelManager/ModelManager.h"

class ModelResources {
public:
	// シングルトン
	static ModelResources* GetInstance();
	// model読み込む場所
	void LoadModel();

private:
	ModelResources() = default;
	~ModelResources() = default;
	ModelResources(const ModelResources&) = delete;
	ModelResources& operator=(const ModelResources&) = delete;
};
#pragma once
