#include "Object3d.hlsli"
struct TransformationMatrix
{
    float32_t4x4 matWorld;
    float32_t4x4 world;
    float32_t4x4 WorldInverseTranspose;
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
};


VertexShaderOutput main(VertexShaderInput input, uint32_t instanceId : SV_InstanceID)
{
    VertexShaderOutput output;
    float32_t4x4 wvp[instanceId] = mul(gTransformationMatrix.matWorld[instanceId], mul(gCameraMatrix.view, gCameraMatrix.projection));
    output.position = mul(input.position, wvp[instanceId]);
    output.texcoord = input.texcoord;
    output.normal = normalize(mul(input.normal, (float32_t3x3) gTransformationMatrix.world[instanceId]));
    return output;
}