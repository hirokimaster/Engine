
struct Well
{
    float32_t4x4 skeletonSpaceMatrix;
    float32_t4x4 skeletonSpaceInverseTransposeMatrix;
};

struct Vertex
{
    float32_t4 position;
    float32_t2 texcoord;
    float32_t3 normal;
};

struct VertexInfluence
{
    float32_t4 weight;
    int32_t4 index;
};

struct SkinningInformation
{
    uint32_t numVertices;
};

// palette
StructuredBuffer<Well> gMatrixPalette : register(t0);
// vertexBufferViewの入力頂点
StructuredBuffer<Vertex> gInputVertices : register(t1);
// vertexBufferViewの入力インフルエンス
StructuredBuffer<VertexInfluence> gInfluences : register(t2);
// skinning計算後の頂点データ
RWStructuredBuffer<Vertex> gOutputVertices : register(u0);
// skinningに関するちょっとした情報
ConstantBuffer<SkinningInformation> gSkinningInformation : register(b0);

[numthreads(1024, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    uint32_t vertexIndex = DTid.x;
    if (vertexIndex < gSkinningInformation.numVertices)
    {
        // skinning計算
        // 必要なデータをstructuredBufferから取ってくる
        Vertex input = gInputVertices[vertexIndex];
        VertexInfluence influence = gInfluences[vertexIndex];
        
        // skinning後の頂点を計算
        Vertex skinned;
        skinned.texcoord = input.texcoord;
        
         // 位置の変換
        skinned.position = mul(input.position, gMatrixPalette[influence.index.x].skeletonSpaceMatrix) * influence.weight.x;
        skinned.position += mul(input.position, gMatrixPalette[influence.index.y].skeletonSpaceMatrix) * influence.weight.y;
        skinned.position += mul(input.position, gMatrixPalette[influence.index.z].skeletonSpaceMatrix) * influence.weight.z;
        skinned.position += mul(input.position, gMatrixPalette[influence.index.w].skeletonSpaceMatrix) * influence.weight.w;
        skinned.position.w = 1.0f;
    
        // 法線の変換
        skinned.normal = mul(input.normal, (float32_t3x3) gMatrixPalette[influence.index.x].skeletonSpaceInverseTransposeMatrix) * influence.weight.x;
        skinned.normal += mul(input.normal, (float32_t3x3) gMatrixPalette[influence.index.y].skeletonSpaceInverseTransposeMatrix) * influence.weight.y;
        skinned.normal += mul(input.normal, (float32_t3x3) gMatrixPalette[influence.index.z].skeletonSpaceInverseTransposeMatrix) * influence.weight.z;
        skinned.normal += mul(input.normal, (float32_t3x3) gMatrixPalette[influence.index.w].skeletonSpaceInverseTransposeMatrix) * influence.weight.w;
        skinned.normal = normalize(skinned.normal);
        
        // skininng後の頂点データを格納
        gOutputVertices[vertexIndex] = skinned;
    }
}
