#include "Particle.hlsli"

// 最大数
static const uint32_t kMaxParticles = 1024;

RWStructuredBuffer<Particle> gParticles : register(u0);

RWStructuredBuffer<int32_t> gFreeCounter : register(u1);

[numthreads(1024, 1, 1)]
void main(uint32_t3 DTid : SV_DispatchThreadID )
{
    uint32_t particleIndex = DTid.x;
    if (particleIndex < kMaxParticles)
    {
        // particle構造体の全要素を0で埋めるという書き方
        gParticles[particleIndex].scale = float32_t3(0.5f, 0.5f, 0.5f);
        gParticles[particleIndex].color = float32_t4(1.0f, 1.0f, 1.0f, 1.0f);
    }
    
    if (particleIndex == 0)
    {
        gFreeCounter[0] = 0;
    }
}