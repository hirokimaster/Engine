#include "PostProcess.hlsli"

struct PixelShaderOutput
{
    float32_t4 color : SV_TARGET0;

};

struct VignetteParam
{
    float32_t scale;
    float32_t exponent;
};

ConstantBuffer<VignetteParam> gVignette : register(b0);

Texture2D<float32_t4> gTexture : register(t0);

SamplerState gSampler : register(s0);

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    output.color = gTexture.Sample(gSampler, input.texcoord);
    // 周囲を0に、中心になるほど明るくなるように計算
    float32_t2 correct = input.texcoord * (1.0f - input.texcoord.xy);
    float32_t vignette = correct.x * correct.y * gVignette.scale;
    vignette = saturate(pow(vignette, gVignette.exponent));
    output.color.rgb *= vignette;
    return output;
}