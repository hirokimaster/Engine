/**
* @file ObjectManager.cpp
* @brief objectの一括管理
* @author 仁平 琉乃
*/

#include "ObjectManager.h"

ObjectManager* ObjectManager::GetInstance()
{
    static ObjectManager instance;
    return &instance;
}

void ObjectManager::Initialize()
{
    // 空にしておく
    ClearObject();
}

void ObjectManager::Update()
{
    // インスタンシング用
    for (auto& obj : instancingObjects_) {
        Object3dPlacer& object = *obj.second;
        object.Update();
    }
}

void ObjectManager::Draw(const Camera& camera)
{
    // インスタンシング
    for (auto& obj : instancingObjects_) {
        Object3dPlacer& object = *obj.second;
        object.Draw(camera);
    }

    // 非インスタンシング
    for (auto& obj : individualObjects_) {
        obj->Draw(camera);
    }
}

std::shared_ptr<Object3dInstancing> ObjectManager::CreateInstancingObject(const std::string& modelName, uint32_t texHandle)
{
    // 指定したモデルでobjectのインスタンスがあるか調べる
    auto it = instancingObjects_.find(modelName);
    if (it == instancingObjects_.end()) {
        // 無かったら新しく作る
        std::unique_ptr<Object3dPlacer> object = std::make_unique<Object3dPlacer>();
        object->Initialize(true);
        object->SetModel(modelName);
        object->SetTexHandle(texHandle);
        // 新しく追加したオブジェクトは追加しないように保持する
        it = instancingObjects_.emplace(modelName, std::move(object)).first;
    }

    Object3dPlacer& object = *it->second;
    // 新しいデータ
    auto data = std::make_shared<Object3dInstancing>();
    data->worldTransform.Initialize();
    data->texHandel = texHandle;
    data->isAlive = true;
    data->color = { 1.0f, 1.0f, 1.0f, 1.0f };
    object.SetObject3dInstancing(data);

    return data;
}

std::shared_ptr<Object3dPlacer> ObjectManager::CreateIndividualObject(const std::string& modelName, uint32_t texHandle)
{
    std::shared_ptr<Object3dPlacer> object = std::make_shared<Object3dPlacer>();
    object->Initialize();
    object->SetModel(modelName);
    object->SetTexHandle(texHandle);
    individualObjects_.push_back(object);

    return object;
}

void ObjectManager::ClearObject()
{
    instancingObjects_.clear();
    individualObjects_.clear();
}
