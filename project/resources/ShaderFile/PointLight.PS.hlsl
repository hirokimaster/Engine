#include "Object3d.hlsli"
struct Material
{
    float32_t4 color;
    int32_t enableLighting;
    float32_t shininess;
};

struct DirectionalLight
{
    float32_t4 color;
    float32_t3 direction;
    float intensity;
};

struct PointLight
{
    float32_t4 color;
    float32_t3 position;
    float intensity;
    float radius;
    float decay;
};

struct Camera
{
    float32_t3 worldPosition;
}; 

ConstantBuffer<Material> gMaterial : register(b0);
struct PixelShaderOutput
{
    float32_t4 color : SV_TARGET0;
	
};

ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);

ConstantBuffer<Camera> gCamera : register(b2);

ConstantBuffer<PointLight> gPointLight : register(b3);

Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    float32_t4 textureColor = gTexture.Sample(gSampler, input.texcoord);
    float32_t3 toEye = normalize(gCamera.worldPosition - input.worldPosition);
    float32_t distance = length(gPointLight.position - input.worldPosition);
    float32_t factor = pow(saturate(-distance / gPointLight.radius + 1.0f), gPointLight.decay);
    /*------------------------
        directionalLight 用
    ---------------------------*/
    //float32_t3 reflectLight = reflect(gDirectionalLight.direction, normalize(input.normal));
    float32_t3 halfVector = normalize(-gDirectionalLight.direction + toEye);
    /*------------------------
        PointLightLight 用
    ---------------------------*/
    float32_t3 pointLightDirection = normalize(input.worldPosition - gPointLight.position);
    //float32_t3 pReflectLight = reflect(pointLightDirection, normalize(input.normal));
    float32_t3 pHalfVector = normalize(-pointLightDirection + toEye);
   
    if (gMaterial.enableLighting != 0)
    {
        /*------------------------
             directionalLight 用
        ---------------------------*/
	    // half lambert
        float NdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
        float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
        float32_t3 diffuse = gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity * factor;

        // 鏡面反射
        float NDotH = dot(normalize(input.normal), halfVector);
        float specularPow = pow(saturate(NDotH), gMaterial.shininess); // 反射強度
        float32_t3 specular = gDirectionalLight.color.rgb * gDirectionalLight.intensity * factor * specularPow * float32_t3(1.0f,1.0f,1.0f);

        /*------------------------
             PointLightLight 用
        ---------------------------*/
        float pNdotL = dot(normalize(input.normal), -pointLightDirection);
        float pCos = pow(pNdotL * 0.5f + 0.5f, 2.0f);
        float pNDotH = dot(normalize(input.normal), pHalfVector);
        float pSpecularPow = pow(saturate(pNDotH), gMaterial.shininess); // 反射強度
        float32_t3 pDiffuse = gMaterial.color.rgb * textureColor.rgb * gPointLight.color.rgb * pCos * gPointLight.intensity * factor;
        float32_t3 pSpecular = gPointLight.color.rgb * gPointLight.intensity * factor * pSpecularPow * float32_t3(1.0f,1.0f,1.0f);

        output.color.rgb = diffuse + specular + pDiffuse + pSpecular;
        output.color.a = gMaterial.color.a * textureColor.a;
    }
    else
    {
        output.color = gMaterial.color * textureColor;
    }

    return output;

};