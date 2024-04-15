#pragma once
#include "engine/Transform/WorldTransform.h"
#include "engine/GraphicsPipeline/GraphicsPipeline.h"
#include "engine/TextureManager/TextureManager.h"
#include "engine/Camera/Camera.h"

class Model;

class IModelState {
public:
    /// <summary>
    /// 初期化
    /// </summary>
    /// <param name="state"></param>
    virtual void Initialize(Model* state) = 0;


    /// <summary>
    /// 描画
    /// </summary>
    virtual void Draw(WorldTransform worldTransform, Camera camera, uint32_t texHandle) = 0;

};
