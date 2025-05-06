#include "Particle.hlsli"

// 最大数
static const uint32_t kMaxParticles = 1024;

float32_t setRange(float32_t random, float32_t min, float32_t max)
{
    return min + random * (max - min);
}

float32_t3 setRange3d(float32_t3 random, float32_t3 min, float32_t3 max)
{
    return float32_t3(
            setRange(random.x, min.x, max.x),
            setRange(random.y, min.y, max.y),
            setRange(random.z, min.z, max.z)
        );
}

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
        rand3dTo1d(value, float32_t3(73.156, 52.235, 09.151))
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
    
    float32_t ApplyRange1d(float32_t min, float32_t max)
    {
        float32_t value = Generate1d();
        return setRange(value, min, max);
    }

    float32_t3 ApplyRange3d(float32_t3 min, float32_t3 max)
    {
        float32_t3 value = Generate3d();
        return setRange3d(value, min, max);
    
    }
};

struct Range1d
{
    float32_t min;
    float32_t max;
};

struct Range3d
{
    float32_t3 min;
    float32_t3 max;
};

struct EmitterSphere
{
    float32_t3 translate;
    float32_t3 velocity;
    float32_t radius;
    uint32_t count;
    float32_t frequency;
    float32_t frequencyTime;
    uint32_t emit;
    Range3d rangeTranslate;
    Range3d rangeScale;
    Range1d rangeLifeTime;
    Range3d rangeVelocity;
    Range1d rangeCurrentTime;
    Range3d rangeColor;
    Range1d rangeAlpha;
};

ConstantBuffer<EmitterSphere> gEmitter : register(b0);

ConstantBuffer<PerFrame> gPerFrame : register(b1);

RWStructuredBuffer<Particle> gParticles : register(u0);

RWStructuredBuffer<int32_t> gFreeListIndex : register(u1);

RWStructuredBuffer<uint32_t> gFreeList : register(u2);

[numthreads(1, 1, 1)]
void main(uint32_t3 DTid : SV_DispatchThreadID)
{
    if (gEmitter.emit != 0)
    {
        RandomGenerator generator;
        generator.seed = (DTid + gPerFrame.time) * gPerFrame.time;
       
        for (uint32_t countIndex = 0; countIndex < gEmitter.count; ++countIndex)
        {
            int32_t freeListIndex;
            InterlockedAdd(gFreeListIndex[0], -1, freeListIndex);
            if (0 <= freeListIndex && freeListIndex < kMaxParticles)
            {
                uint32_t particleIndex = gFreeList[freeListIndex];
                gParticles[particleIndex].scale = generator.ApplyRange3d(gEmitter.rangeScale.min, gEmitter.rangeScale.max);
                gParticles[particleIndex].translate = gEmitter.translate + generator.ApplyRange3d(gEmitter.rangeTranslate.min,gEmitter.rangeTranslate.max);
                gParticles[particleIndex].color.rgb = generator.ApplyRange3d(gEmitter.rangeColor.min,gEmitter.rangeColor.max);
                gParticles[particleIndex].color.a = generator.ApplyRange1d(gEmitter.rangeAlpha.min, gEmitter.rangeAlpha.max);
                gParticles[particleIndex].velocity = gEmitter.velocity + generator.ApplyRange3d(gEmitter.rangeVelocity.min,gEmitter.rangeVelocity.max);
                gParticles[particleIndex].lifeTime = generator.ApplyRange1d(gEmitter.rangeLifeTime.min, gEmitter.rangeLifeTime.max);
                gParticles[particleIndex].currentTime = generator.ApplyRange1d(gEmitter.rangeCurrentTime.min, gEmitter.rangeCurrentTime.max);

            }
            else
            {
                InterlockedAdd(gFreeListIndex[0], 1);
                break;
            }
            
        }

    }
}