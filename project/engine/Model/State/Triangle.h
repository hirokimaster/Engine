#pragma once
#include "engine/Base/DX/DirectXCommon.h"
#include "engine/Utility/StringUtility.h"
#include "engine/Utility/ShaderCompile.h"
#include "engine/CreateResource/CreateResource.h"
#include "engine/GraphicsPipeline/GraphicsPipeline.h"
#include "engine/Math/Mathfunction.h"
#include "engine/Transform/WorldTransform.h"
#include "engine/Camera/Camera.h"
#include "engine/TextureManager/TextureManager.h"

class Triangle {
public:
	void Initialize();

	void Draw(WorldTransform worldTransform, Camera camera, uint32_t texHandle);

private:

	D3D12_VERTEX_BUFFER_VIEW VBV{};
	Resource resource_ = {};
	WorldTransform worldTransform_ = {};

};
