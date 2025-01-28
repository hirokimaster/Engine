#include "BaseObject.h"

BaseObject::~BaseObject()
{
}

void BaseObject::Initialize()
{
	object_ = std::make_unique<Object3DPlacer>();
	object_->Initialize();
}

void BaseObject::Update()
{
	object_->Update();
}

void BaseObject::Draw(Camera& camera)
{
	object_->Draw(camera);
}
