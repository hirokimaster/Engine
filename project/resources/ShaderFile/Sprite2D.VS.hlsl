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

ConstantBuffer<TransformationMatrix> gTransformationMatrix : register(b0);

ConstantBuffer<CameraMatrix> gCameraMatrix : register(b1);

struct VertexShaderInput
{
    float32_t4 position : POSITION0;
    float32_t2 texcoord : TEXCOORD0;
};


VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;
    float32_t4x4 wvp = mul(gTransformationMatrix.matWorld, mul(gCameraMatrix.view, gCameraMatrix.projection));
    output.position = mul(input.position, wvp);
    output.texcoord = input.texcoord;
    return output;
}