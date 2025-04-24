#include "ObjectManager.h"

void ObjectManager::Initialize()
{
    // 空にしておく
    ClearObjectMap();
}

void ObjectManager::Update()
{
    for (auto& obj : objectMap_) {
        Object3DPlacer& object = *obj.second;
        object.Update();
    }
}

void ObjectManager::Draw(const Camera& camera)
{
    for (auto& obj : objectMap_) {
        Object3DPlacer& object = *obj.second;
        object.Draw(camera);
    }
}

std::shared_ptr<Object3dData> ObjectManager::CreateInstance(const std::string& modelName, uint32_t texHandle)
{
    // 指定したモデルでobjectのインスタンスがあるか調べる
    auto it = objectMap_.find(modelName);
    if (it == objectMap_.end()) {
        // 無かったら新しく作る
        std::unique_ptr<Object3DPlacer> object = std::make_unique<Object3DPlacer>();
        object->Initialize(true);
        object->SetModel(modelName);
        // 新しく追加したオブジェクトは追加しないように保持する
        it = objectMap_.emplace(modelName, std::move(object)).first;
    }

    Object3DPlacer& object = *it->second;
    // 新しいデータ
    auto data = std::make_shared<Object3dData>();
    data->worldTransform.Initialize();
    data->texHandel = texHandle;
    data->color = { 1.0f, 1.0f, 1.0f, 1.0f };
    object.SetObject3dData(data);

    return data;
}

void ObjectManager::ClearObjectMap()
{
    objectMap_.clear();
}
