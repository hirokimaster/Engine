#include "Primitive.h"

Primitive::Primitive(){}

Primitive::~Primitive()
{
	delete state_;
}

void Primitive::Initialize(IPrimitive* state)
{
	state_ = state;
	state_->Initialize(this);
}

void Primitive::Draw(WorldTransform worldTransform, Camera& camera)
{
	state_->Draw(worldTransform, camera);
}
