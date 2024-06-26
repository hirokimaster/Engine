#include "BaseCharacter.h"

void BaseCharacter::Initialize(Object3DPlacer* object, uint32_t texHandle, const std::string& model)
{
	object_ = object;
	object_->Initialize();
	object_->SetModel(model);
	object_->SetTexHandle(texHandle);
    
	worldTransform_.Initialize();
}

void BaseCharacter::Update()
{
	worldTransform_.UpdateMatrix();
}

void BaseCharacter::Draw(Camera& camera)
{
	object_->Draw(worldTransform_, camera);
}