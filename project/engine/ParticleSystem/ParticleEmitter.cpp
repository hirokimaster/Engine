#include "ParticleEmitter.h"

ParticleEmitter::ParticleEmitter()
{
}

ParticleEmitter::~ParticleEmitter()
{
}

void ParticleEmitter::Initialize()
{
	// emitter用のbuffer作成
	CreateBuffer();
	// particleManagerのインスタンスを借りる
	particleManager_ = ParticleManager::GetInstance();
	
}

void ParticleEmitter::Update()
{

}

void ParticleEmitter::Emit()
{
	
}

void ParticleEmitter::CreateBuffer()
{
	// emitterのリソース作成
	emitterSphereResource_ = CreateResource::CreateBufferResource(sizeof(EmitterSphere));
	// データを書き込む
	emitterSphereResource_->Map(0, nullptr, reinterpret_cast<void**>(&emitterSphereData_));
}

void ParticleEmitter::Unmap()
{
	emitterSphereResource_->Unmap(0, nullptr);
}

