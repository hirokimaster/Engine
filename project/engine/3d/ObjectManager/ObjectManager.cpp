#include "ObjectManager.h"

void ObjectManager::Initialize()
{
	objects_.clear();
}

void ObjectManager::Update()
{
	for (auto& object : objects_) {
		object->Update();
	}
}

void ObjectManager::Draw(const Camera& camera)
{
	for (auto& object : objects_) {
		object->Draw(camera);
	}
}
