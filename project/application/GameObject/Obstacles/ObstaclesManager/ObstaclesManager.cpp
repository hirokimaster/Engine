#include "ObstaclesManager.h"

void ObstaclesManager::Initialize()
{
	// レーザー
	auto it = loader_->GetObjectDatas().find("laser");
	if (it != loader_->GetObjectDatas().end()) {
		for (auto& objectData : it->second) {
			std::unique_ptr<Laser> object = std::make_unique<Laser>();
			object->Initialize();
			object->SetPosition(objectData.translate);
			object->SetScale(objectData.scale);
			lasers_.push_back(std::move(object));
		}
	}

	// 壁
	auto it2 = loader_->GetObjectDatas().find("wall");
	if (it2 != loader_->GetObjectDatas().end()) {
		for (auto& objectData : it2->second) {
			std::unique_ptr<Wall> object = std::make_unique<Wall>();
			object->Initialize();
			object->SetPosition(objectData.translate);
			object->SetScale(objectData.scale);
			obstacles_.push_back(std::move(object));
		}
	}
}

void ObstaclesManager::Update()
{
	// 更新
	for (auto& obj : obstacles_) {
		obj->Update();
	}

	for (auto& laser : lasers_) {
		laser->Update();
	}
}
