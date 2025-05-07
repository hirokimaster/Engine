/**
* @file BaseIndividualObject.cpp
* @brief 単一オブジェクトの基底クラス
* @author 仁平 琉乃
*/

#include "BaseIndividualObject.h"

BaseIndividualObject::~BaseIndividualObject()
{
}

void BaseIndividualObject::Initialize(const std::string& modelName, const std::string& textureName, ColliderType type)
{
	objectManager_ = ObjectManager::GetInstance();
	object_ = objectManager_->CreateIndividualObject(modelName, TextureManager::GetTexHandle(textureName));

	// 形状だけ設定する
	// 形状を指定しなかったら作らない
	if (type != ColliderType::None) {
		collider_ = std::make_unique<Collider>();
		collider_->SetType(type); // 形状
	}
}

void BaseIndividualObject::Update()
{
	if (auto obj = object_.lock()) {
		obj->Update();
	}
}