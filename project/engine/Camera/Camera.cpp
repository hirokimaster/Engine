#include "Camera.h"

void Camera::Initialize() {

	CreateConstBuffer();
	Map();
	UpdateMatrix();
	TransferMatrix();
	sMatProjection = MakeIdentityMatrix();
}

void Camera::CreateConstBuffer()
{
	constBuff_ = CreateResource::CreateBufferResource(sizeof(ConstBufferDataViewProjection));
	sConstBuff = CreateResource::CreateBufferResource(sizeof(ConstBufferDataViewProjectionSprite));
}

void Camera::Map()
{
	constBuff_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&constMap));
	sConstBuff.Get()->Map(0, nullptr, reinterpret_cast<void**>(&sConstMap));
}

void Camera::UpdateMatrix() {

	matView = Inverse(MakeAffineMatrix(scale, rotate, translate));
	matProjection = MakePerspectiveFovMatrix(fov, aspectRatio, nearZ, farZ);

	sMatView = MakeIdentityMatrix();
	sMatProjection = MakeOrthographicMatrix(0.0f, 0.0f, 1280.0f, 720.0f, 0.0f, 100.0f);
	TransferMatrix();

}

void Camera::TransferMatrix()
{
	constMap->view = matView;
	constMap->projection = matProjection;
	constMap->cameraPos = { matView.m[3][0],matView.m[3][1],matView.m[3][2] };

	sConstMap->view = sMatView;
	sConstMap->projection = sMatProjection;
	sConstMap->cameraPos = { matView.m[3][0],matView.m[3][1],matView.m[3][2] };

}