#include "ModelManager.h"

ModelManager* ModelManager::GetInstance()
{
	static ModelManager instance;
	return &instance;
}

void ModelManager::LoadObjModel(const std::string& fileName)
{
	// 読み込み済みなら抜ける
	if (ModelManager::GetInstance()->models_.contains(fileName)) {
		return;
    }

	std::unique_ptr<Model> model = std::make_unique<Model>();
	model->InitializeObj(fileName);
	ModelManager::GetInstance()->models_.insert(std::make_pair(fileName, std::move(model)));

}

void ModelManager::LoadGLTFModel(const std::string& fileName)
{
	// 読み込み済みなら抜ける
	if (ModelManager::GetInstance()->models_.contains(fileName)) {
		return;
	}

	std::unique_ptr<Model> model = std::make_unique<Model>();
	model->InitializeGLTF(fileName);
	ModelManager::GetInstance()->models_.insert(std::make_pair(fileName, std::move(model)));
}

Model* ModelManager::CreateObj(const std::string& fileName)
{
	if (ModelManager::GetInstance()->models_.contains(fileName)) {
		return ModelManager::GetInstance()->models_.at(fileName).get();
	}

	return nullptr;
}

Model* ModelManager::CreateGLTF(const std::string& fileName)
{
	if (ModelManager::GetInstance()->models_.contains(fileName)) {
		return ModelManager::GetInstance()->models_.at(fileName).get();
	}

	return nullptr;
}
