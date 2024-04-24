#include "WorldTransform.h"

//void WorldTransform::Initialize(){
//
//	matWorld = MakeIdentityMatrix();
//	sMatWorld = MakeIdentityMatrix();
//	sWorldMatrix = MakeIdentityMatrix();
//}
//
//void WorldTransform::TransferMatrix(Microsoft::WRL::ComPtr<ID3D12Resource>& wvpResource, Camera& camera){
//
//	TransformationMatrix* wvp = {};
//	matWorld = Multiply(matWorld, Multiply(camera.matView, camera.matProjection));
//	wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvp));
//	wvp->WVP = matWorld;
//	wvp->World = worldMatrix;
//
//}
//
//void WorldTransform::STransferMatrix(Microsoft::WRL::ComPtr<ID3D12Resource>& wvpResource, Camera& camera)
//{
//	TransformationMatrix* wvp = {};
//	sMatWorld = Multiply(matWorld, Multiply(camera.sMatView, camera.sMatProjection));
//	wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvp));
//	wvp->WVP = sMatWorld;
//	wvp->World = worldMatrix;
//}
//
//void WorldTransform::AssimpTransferMatrix(Microsoft::WRL::ComPtr<ID3D12Resource>& wvpResource, Matrix4x4 localMatrix, Camera& camera)
//{
//	TransformationMatrix* wvp = {};
//	matWorld = Multiply(matWorld, Multiply(camera.matView, camera.matProjection));
//	wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvp));
//	wvp->WVP = Multiply(localMatrix, matWorld);
//	wvp->World = Multiply(localMatrix, worldMatrix);
//}
//
//void WorldTransform::UpdateMatrix(){
//
//	matWorld = MakeAffineMatrix(scale, rotate, translate);
//	worldMatrix = MakeAffineMatrix(scale, rotate, translate);
//
//	if (parent) {
//		matWorld = Multiply(matWorld, parent->matWorld);
//	}
//
//}
//

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
	constMap->WorldInverseTranspose = Inverse(matWorld);
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
