#include "BaseObject.h"


void BaseObject::Initialize(const std::string& modelName, const std::string& textureName, ColliderType type)
{
	// object生成
	object_ = std::make_unique<Object3DPlacer>();
	object_->Initialize();
	object_->SetModel(modelName);
	object_->SetTexHandle(TextureManager::GetTexHandle(textureName));
	object_->SetColor({ 1.0f,1.0f,1.0f,1.0f });

	// 形状だけ設定する
	// 形状を指定しなかったら作らない
	if (type != ColliderType::None) {
		collider_ = std::make_unique<Collider>();
		collider_->SetType(type); // 形状
	}
	
}

void BaseObject::Update()
{
	object_->Update();
}

void BaseObject::Draw(const Camera& camera)
{
	object_->Draw(camera);
}
