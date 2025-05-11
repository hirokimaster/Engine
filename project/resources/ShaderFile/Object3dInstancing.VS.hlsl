#include "Object3dInstancing.hlsli"
struct TransformationMatrix
{
    float32_t4x4 matWorld;
    float32_t4x4 world;
    float32_t4x4 WorldInverseTranspose;
    float32_t4 color;
};

struct CameraMatrix
{
    float32_t4x4 view;
    float32_t4x4 projection;
    float32_t3 worldPosition;
};

StructuredBuffer<TransformationMatrix> gTransformationMatrix : register(t0);

ConstantBuffer<CameraMatrix> gCameraMatrix : register(b0);

struct VertexShaderInput
{
    float32_t4 position : POSITION0;
    float32_t2 texcoord : TEXCOORD0;
    float32_t3 normal : NORMAL0;
    float32_t4 color : COLOR0;
};


VertexShaderOutput main(VertexShaderInput input, uint32_t instanceId : SV_InstanceID)
{
    VertexShaderOutput output;
    float32_t4x4 wvp = mul(gTransformationMatrix[instanceId].matWorld, mul(gCameraMatrix.view, gCameraMatrix.projection));
    output.position = mul(input.position, wvp);
    output.texcoord = input.texcoord;
    output.normal = normalize(mul((float32_t3x3) gTransformationMatrix[instanceId].world, input.normal));
    output.color = gTransformationMatrix[instanceId].color;
    return output;
}