/**
* @file InstanceWorldTransform.cpp
* @brief instancing用のワールド座標
* @author 仁平 琉乃
*/

#include "InstanceWorldTransform.h"

void InstanceWorldTransform::Initialize()
{
	// 単位行列を入れておく
	world = MakeIdentityMatrix();
	matWorld = MakeIdentityMatrix();
}

void InstanceWorldTransform::UpdateMatrix()
{
	matWorld = MakeAffineMatrix(scale, rotate, translate);
	// 親があれば親のワールド行列を掛ける
	if (parent) {
		matWorld = Multiply(matWorld, parent->matWorld);
	}
}
