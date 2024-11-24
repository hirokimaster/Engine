#include "Particle.hlsli"

// 最大数
static const uint32_t kMaxParticles = 1024;

float32_t rand3dTo1d(float32_t3 value, float32_t3 dotDir = float32_t3(12.9898, 78.233, 37.719))
{
    // 各成分に sin を適用
    float32_t3 smallValue = sin(value);

    // smallValue と dotDir の内積を計算
    float32_t random = dot(smallValue, dotDir);

    // sin の結果にスケールを掛けて乱数を生成
    random = frac(sin(random) * 143758.5453);

    return random;
}

// 3Dベクトルをランダムな3Dベクトルに変換する関数
float32_t3 rand3dTo3d(float32_t3 value)
{
    return float32_t3(
        rand3dTo1d(value, float32_t3(12.989, 78.233, 37.719)),
        rand3dTo1d(value, float32_t3(39.346, 11.135, 83.155)),
        rand3dTo1d(value, float32_t3(73.156, 52.235, 9.151))
    );
}


class RandomGenerator
{
    float32_t3 seed;
    float32_t3 Generate3d()
    {
        seed = rand3dTo3d(seed);
        return seed;
    }
    
    float32_t Generate1d()
    {
        float32_t result = rand3dTo1d(seed);
        seed.x = result;
        return result;
    }
};

struct EmitterSphere
{
    float32_t3 translate;
    float32_t radius;
    uint32_t count;
    float32_t frequency;
    float32_t frequencyTime;
    uint32_t emit;
};

ConstantBuffer<EmitterSphere> gEmitter : register(b0);

ConstantBuffer<PerFrame> gPerFrame : register(b1);

RWStructuredBuffer<Particle> gParticles : register(u0);

RWStructuredBuffer<int32_t> gFreeCounter : register(u1);

[numthreads(1, 1, 1)]
void main(uint32_t DTid : SV_DispatchThreadID)
{
    if (gEmitter.emit != 0)
    {
        RandomGenerator generator;
        generator.seed = (DTid + gPerFrame.time) * gPerFrame.time;
        
        for (uint32_t countIndex = 0; countIndex < gEmitter.count; ++countIndex)
        {
            int32_t particleIndex;
            InterlockedAdd(gFreeCounter[0], 1, particleIndex);
            if (particleIndex < kMaxParticles)
            {
                // カウント分のParticleを射出する
                gParticles[countIndex].scale = generator.Generate3d();
                gParticles[countIndex].translate = generator.Generate3d();
                gParticles[countIndex].color.rgb = generator.Generate3d();
                gParticles[countIndex].color.a = 1.0f;
                gParticles[countIndex].velocity = generator.Generate3d();
                gParticles[countIndex].lifeTime = generator.Generate1d();
                gParticles[countIndex].currentTime = generator.Generate1d();
            }
            
        }

    }
}