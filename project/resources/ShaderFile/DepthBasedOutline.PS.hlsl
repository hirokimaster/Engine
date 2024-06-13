#include "PostProcess.hlsli"

static const float32_t2 kIndex3x3[3][3] =
{
    { { -1.0f, -1.0f }, { 0.0f, -1.0f }, { 1.0f, -1.0f } },
    { { -1.0f, 0.0f }, { 0.0f, 0.0f }, { 1.0f, 0.0f } },
    { { -1.0f, 1.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f } },
};

static const float32_t kPrewittHorizontalKernel[3][3] =
{
    { -1.0f / 6.0f, 0.0f, 1.0f / 6.0f },
    { -1.0f / 6.0f, 0.0f, 1.0f / 6.0f },
    { -1.0f / 6.0f, 0.0f, 1.0f / 6.0f },
};

static const float32_t kPrewittVerticalKernel[3][3] =
{
    { -1.0f / 6.0f, -1.0f / 6.0f, -1.0f / 6.0f },
    { 0.0f, 0.0f, 0.0f },
    { 1.0f / 6.0f, 1.0f / 6.0f, 1.0f / 6.0f },
};

struct PixelShaderOutput
{
    float32_t4 color : SV_TARGET0;
	
};

struct Projection
{
    float32_t4x4 projectionInverse;
};

Texture2D<float32_t4> gTexture : register(t0);

SamplerState gSampler : register(s0);

Texture2D<float32_t> gDepthTexture : register(t1);

SamplerState gSamplerPoint : register(s1);

ConstantBuffer<Projection> gProjection: register(b0);

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    
    
    float32_t DepthTex = gDepthTexture.Sample(gSamplerPoint, input.texcoord);
    
    uint32_t width, height;
    gTexture.GetDimensions(width, height);
    float32_t2 uvStepSize = float32_t2(rcp(width), rcp(height));
    float32_t2 difference = float32_t2(0.0f, 0.0f); // 畳み込みの結果を格納する
    
    // 色を輝度に変換して、畳み込みを行う
    for (int32_t x = 0; x < 3; ++x)
    {
        for (int32_t y = 0; y < 3; ++y)
        {
            float32_t2 texcoord = input.texcoord + kIndex3x3[x][y] * uvStepSize;
            
            float32_t ndcDepth = gDepthTexture.Sample(gSamplerPoint, texcoord);
            float32_t4 viewSpace = mul(float32_t4(0.0f, 0.0f, ndcDepth, 1.0f), gProjection.projectionInverse);
            float32_t viewZ = viewSpace.z * rcp(viewSpace.w);
            
            difference.x += viewZ * kPrewittHorizontalKernel[x][y];
            difference.y += viewZ * kPrewittVerticalKernel[x][y];
        }

    }
    
    // 変化の長さをウェイトとして合成
    float32_t weight = length(difference);
    weight = saturate(weight);
    
    output.color.rgb = (1.0f - weight) * gTexture.Sample(gSampler, input.texcoord).rgb;
    output.color.a = 1.0f;
    return output;

}