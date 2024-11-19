#include "GPUParticle.h"

void GPUParticle::CreateUAV()
{
	// uavのresource作成
	uavResource_ = CreateResource::CreateUAVResource(sizeof(GPUParticle) * 1024);

}
