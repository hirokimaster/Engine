#include "PostProcess.hlsli"

struct PixelShaderOutput
{
    float32_t4 color : SV_TARGET0;
	
};

Texture2D<float32_t4> gTexture : register(t0);

Texture2D<float32_t> gMaskTexture : register(t1);

SamplerState gSampler : register(s0);

struct DissolveParam
{
    float32_t threshold;
};

ConstantBuffer<DissolveParam> gDissolve : register(b0);

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    
    float32_t mask = gMaskTexture.Sample(gSampler, input.texcoord);
    // maskの値0.5(閾値)以下の場合はdiscardして抜く
    if (mask <= gDissolve.threshold)
    {
        discard;
    }
    
    output.color = gTexture.Sample(gSampler, input.texcoord);
    
    return output;
}