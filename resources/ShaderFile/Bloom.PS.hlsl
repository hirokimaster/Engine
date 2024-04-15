#include "Object3d.hlsli"

struct PixelShaderOutput
{
    float32_t4 color : SV_TARGET0;
	
};

struct Bloom
{
    float32_t stepWidth;
    float32_t sigma;
    float32_t lightStrength;
    float32_t bloomThreshold;
};

ConstantBuffer<Bloom> gBloom : register(b0);

Texture2D<float32_t4> gTexture : register(t0);

SamplerState gSampler : register(s0);

float32_t Gaussian(float32_t2 drawUV, float32_t2 pickUV, float32_t sigma)
{
    float32_t d = distance(drawUV, pickUV);
    return exp(-(d * d) / (2 * sigma * sigma));
}

float32_t4 WeightCalc(float32_t2 texCoord)
{
    float32_t totalWeight = 0;
    float32_t sigma = gBloom.sigma;
    float32_t stepWidth = gBloom.stepWidth;
    float32_t4 col = float32_t4(0, 0, 0, 0);
    
    for (float32_t py = -sigma * 2; py <= sigma * 2; py += stepWidth)
    {
        for (float32_t px = -sigma * 2; px <= sigma * 2; px += stepWidth)
        {
            float32_t2 pickUV = texCoord + float32_t2(px, py);
            float32_t weight = Gaussian(texCoord, pickUV, sigma);
            float32_t4 color = gTexture.Sample(gSampler, pickUV);
            col += color * weight;
            totalWeight += weight;
        }

    }
    
    col.rgb = col.rgb / totalWeight;
    col.a = 1.0f;
    return col;
}

float32_t4 ExtractLuminance(float32_t4 col)
{
    float32_t grayScale = col.r * 0.299 + col.g * 0.587f + col.b * 0.114;
    float32_t extract = smoothstep(0.6, 0.9, grayScale);
    return col * extract;
}

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
   
    // ガウシアンブラー
    float32_t4 gaussian = WeightCalc(input.texcoord.xy);
    
     // 高輝度の部分のみにかける
    float32_t4 LuminanceColor = ExtractLuminance(gaussian);
    
    // 元の色
    float32_t4 normalColor = gTexture.Sample(gSampler, input.texcoord.xy);
    
    // 元の色と高輝度部分を合成する
    float32_t4 LuminanceGaussian = LuminanceColor + normalColor;

    output.color = LuminanceGaussian;
    return output;
}