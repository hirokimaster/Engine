#include "Primitive.h"

void Primitive::Initialize(IPrimitiveState* state)
{
	state_ = state;
	state_->Initialize(this, texHandle_);
}

void Primitive::Draw(WorldTransform& worldTransform, Camera& camera)
{
	state_->Draw(worldTransform, camera);
}
