
// 最大数
static const uint32_t kMaxParticles = 1024;

struct Particle
{
    float32_t3 translate;
    float32_t3 scale;
    float32_t lifeTime;
    float32_t3 velocity;
    float32_t currentTime;
    float32_t4 color;
};

struct PerView
{
    float32_t4x4 viewProjection;
    float32_t4x4 billboardMatrix;
};

RWStructuredBuffer<Particle> gParticles : register(u0);

[numthreads(1024, 1, 1)]
void main(uint32_t3 DTid : SV_DispatchThreadID )
{
    uint32_t particleIndex = DTid.x;
    if (particleIndex < kMaxParticles)
    {
        // particle構造体の全要素を0で埋めるという書き方
        gParticles[particleIndex] = (Particle)0;
    }
}