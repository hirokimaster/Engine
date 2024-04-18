#pragma once
#include <math.h>
#define _USE_MATH_DEFINES
#include "engine/Camera/Camera.h"
#include "engine/Transform/WorldTransform.h"
#include "engine/GraphicsPipeline/GraphicsPipeline.h"
#include "engine/DescriptorManager/SRVManager/SrvManager.h"

class ModelSphere{
public:

	void Initialize();

	void Draw(WorldTransform worldTransform, Camera camera, uint32_t texHandle);

private:

	D3D12_VERTEX_BUFFER_VIEW VBV{};
	Resource resource_ = {};
	WorldTransform worldTransform_ = {};
	// 分割数
	uint32_t kSubdivision = 16;

};