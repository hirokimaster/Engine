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
  
    drawQueue_.clear(); // 忘れずに前フレーム分をクリア

    // インスタンシングオブジェクトを登録
    for (auto& [key, placer] : instancingObjects_) {
        drawQueue_.push_back({
            placer->GetCategory(),
            placer->GetPositionZ(),
            placer.get()
            });
    }

    // 個別オブジェクトを登録
    for (auto& placer : individualObjects_) {
        drawQueue_.push_back({
            placer->GetCategory(),
            placer->GetPositionZ(),
            placer.get()
            });
    }

    // 描画カテゴリ、Z順でソート
    std::sort(drawQueue_.begin(), drawQueue_.end(),
        [](const DrawEntry& a, const DrawEntry& b) {
            if (a.category != b.category) {
                return static_cast<int>(a.category) < static_cast<int>(b.category);
            }
            return a.positionZ < b.positionZ;
        });

    // 描画
    for (auto& entry : drawQueue_) {
        entry.object->Draw(camera);
    }
}

std::shared_ptr<Object3dInstancing> ObjectManager::CreateInstancingObject(const std::string& modelName, uint32_t texHandle, bool shadow)
{
    // 指定したモデルでobjectのインスタンスがあるか調べる
    auto it = instancingObjects_.find(modelName);
    if (it == instancingObjects_.end()) {
        // 無かったら新しく作る
        std::unique_ptr<Object3dPlacer> object = std::make_unique<Object3dPlacer>();
        object->Initialize(true);
        object->SetModel(modelName);
        object->SetTexHandle(texHandle);
        object->SetShadow(shadow);
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
    drawQueue_.clear();
}

