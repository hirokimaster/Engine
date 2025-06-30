/**
* @file BaseInstancingObject.cpp
* @brief インスタンシングオブジェクトの基底クラス
* @author 仁平 琉乃
*/

#include "BaseInstancingObject.h"

BaseInstancingObject::~BaseInstancingObject()
{

}

void BaseInstancingObject::Initialize(const std::string& modelName, const std::string& textureName, ColliderType type,bool shadow)
{
	objectManager_ = ObjectManager::GetInstance();
	object_ = objectManager_->CreateInstancingObject(modelName, TextureManager::GetTexHandle(textureName), shadow);

	// 形状だけ設定する
	// 形状を指定しなかったら作らない
	if (type != ColliderType::None) {
		collider_ = std::make_unique<Collider>();
		collider_->SetType(type); // 形状
	}
}

void BaseInstancingObject::Update()
{
	if (auto obj = object_.lock()) {
		obj->worldTransform.UpdateMatrix();
	}
}