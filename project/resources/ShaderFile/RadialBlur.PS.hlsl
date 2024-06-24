#include "PostProcess.hlsli"

struct PixelShaderOutput
{
    float32_t4 color : SV_TARGET0;
	
};

struct RadialParam
{
    float32_t2 center;
    float32_t blurWidth;
};

Texture2D<float32_t4> gTexture : register(t0);

SamplerState gSampler : register(s0);

ConstantBuffer<RadialParam> gRadial : register(b0);

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    
    const float32_t2 kCenter = float32_t2(gRadial.center); // 中心点
    const int32_t kNumSamples = 10; // サンプリング数。
    const float32_t kBlurWidth = gRadial.blurWidth;
    // 中心からの現在のuv対しての方向を計算
    float32_t2 direction = input.texcoord - kCenter;
    float32_t3 outputColor = float32_t3(0.0f, 0.0f, 0.0f);
    
    for (int32_t sampleIndex = 0; sampleIndex < kNumSamples; ++sampleIndex)
    {
       // サンプリング
        float32_t2 texcoord = input.texcoord + direction * kBlurWidth * float32_t(sampleIndex);
        outputColor.rgb += gTexture.Sample(gSampler, texcoord).rgb;
    }
    
    // 平均化する
    outputColor.rgb *= rcp(kNumSamples);
    
    output.color.rgb = outputColor;
    output.color.a = 1.0f;
    return output;
}