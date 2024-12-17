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

ConstantBuffer<CameraMatrix> gCameraMatrix : register(b0);

StructuredBuffer<TransformationMatrix> gTransformationMatrixInstancing : register(t0);

struct VertexShaderInput
{
    float32_t4 position : POSITION0;
    float32_t2 texcoord : TEXCOORD0;
    float32_t3 normal : NORMAL0;
};


VertexShaderOutput main(VertexShaderInput input, uint32_t instancId : SV_InstanceID)
{
    VertexShaderOutput output;
    // インスタンスごとのワールド座標
    TransformationMatrix transform = gTransformationMatrixInstancing[instanceId];
    // 計算
    float32_t4x4 wvp = mul(transform.matWorld, mul(gCameraMatrix.view, gCameraMatrix.projection));
    output.position = mul(input.position, wvp);
    output.texcoord = input.texcoord;
    output.normal = normalize(mul(input.normal, (float32_t3x3) transform.world));
    return output;
}
