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

struct SpotLight
{
    float32_t4 color;
    float32_t3 position;
    float32_t intensity;
    float32_t3 direction;
    float32_t distance;
    float32_t decay;
    float32_t cosAngle;
    float32_t cosFalloffStart;
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

ConstantBuffer<SpotLight> gSpotLight : register(b3);

Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    float32_t4 textureColor = gTexture.Sample(gSampler, input.texcoord);
    float32_t3 toEye = normalize(gCamera.worldPosition - input.worldPosition);
    float32_t distance = length(gSpotLight.position - input.worldPosition);
    float32_t factor = pow(saturate(-distance / gSpotLight.distance + 1.0f), gSpotLight.decay);
    /*------------------------
        directionalLight 用
    ---------------------------*/
    //float32_t3 reflectLight = reflect(gDirectionalLight.direction, normalize(input.normal));
    float32_t3 halfVector = normalize(-gDirectionalLight.direction + toEye);
    
     /*--------------------------
             SpotLight用
      ---------------------------*/
    float32_t3 spotLightDirectionOnSurface = normalize(input.worldPosition - gSpotLight.position);
    float32_t cosAngle = dot(spotLightDirectionOnSurface, gSpotLight.direction);
    float32_t falloffFactor = saturate((cosAngle - gSpotLight.cosAngle) / (gSpotLight.cosFalloffStart - gSpotLight.cosAngle));
   
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
        float32_t3 specular = gDirectionalLight.color.rgb * gDirectionalLight.intensity * factor * specularPow * float32_t3(1.0f, 1.0f, 1.0f);
        
        /*--------------------------
               SpotLight用
        ---------------------------*/
        float sNdotL = dot(normalize(input.normal), -spotLightDirectionOnSurface);
        float sCos = pow(sNdotL * 0.5f + 0.5f, 2.0f);
        float sNDotH = dot(normalize(input.normal), halfVector);
        float sSpecularPow = pow(saturate(sNDotH), gMaterial.shininess); // 反射強度
       
        float32_t3 sDiffuse = gMaterial.color.rgb * textureColor.rgb * gSpotLight.color.rgb * sCos * gSpotLight.intensity * factor * falloffFactor;
        float32_t3 sSpecular = gSpotLight.color.rgb * gSpotLight.intensity * factor * falloffFactor * sSpecularPow * float32_t3(1.0f, 1.0f, 1.0f);

        output.color.rgb = diffuse + specular + sDiffuse + sSpecular;
        output.color.a = gMaterial.color.a * textureColor.a;
    }
    else
    {
        output.color = gMaterial.color * textureColor;
    }

    return output;

};
