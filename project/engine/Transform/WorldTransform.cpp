#include "WorldTransform.h"

void WorldTransform::Initialize(){

	matWorld = MakeIdentityMatrix();
	sMatWorld = MakeIdentityMatrix();
	sWorldMatrix = MakeIdentityMatrix();
}

void WorldTransform::TransferMatrix(Microsoft::WRL::ComPtr<ID3D12Resource>& wvpResource, Camera& camera){

	TransformationMatrix* wvp = {};
	matWorld = Multiply(matWorld, Multiply(camera.matView, camera.matProjection));
	wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvp));
	wvp->WVP = matWorld;
	wvp->World = worldMatrix;

}

void WorldTransform::STransferMatrix(Microsoft::WRL::ComPtr<ID3D12Resource>& wvpResource, Camera& camera)
{
	TransformationMatrix* wvp = {};
	sMatWorld = Multiply(matWorld, Multiply(camera.sMatView, camera.sMatProjection));
	wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvp));
	wvp->WVP = sMatWorld;
	wvp->World = worldMatrix;
}

void WorldTransform::UpdateMatrix(){

	matWorld = MakeAffineMatrix(scale, rotate, translate);
	worldMatrix = MakeAffineMatrix(scale, rotate, translate);

	if (parent) {
		matWorld = Multiply(matWorld, parent->matWorld);
	}

}

