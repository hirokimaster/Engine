#include "WorldTransform.h"

void WorldTransform::Initialize()
{
	// 単位行列を入れておく
	world = MakeIdentityMatrix();
	matWorld = MakeIdentityMatrix();
	// 定数バッファ作成
	CreateConstBuffer();
	// mapする
	Map();
	// 転送
	TransferMatrix();
	
}

void WorldTransform::CreateConstBuffer()
{
	constBuff = CreateResource::CreateBufferResource(sizeof(ConstBufferDataWorldTransform));
}

void WorldTransform::Map()
{
	constBuff.Get()->Map(0, nullptr, reinterpret_cast<void**>(&constMap));
}

void WorldTransform::TransferMatrix()
{
	constMap->world = world;
	constMap->matWorld = matWorld;
	constMap->WorldInverseTranspose = Transpose(Inverse(matWorld));
}

void WorldTransform::UpdateMatrix()
{
	
	matWorld = MakeAffineMatrix(scale, rotate, translate);
	// 親があれば親のワールド行列を掛ける
	if (parent) {
		matWorld = Multiply(matWorld, parent->matWorld);
	}

	TransferMatrix();
}
