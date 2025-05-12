/**
* @file PlaneProjectionShadow.cpp
* @brief 影
* @author 仁平 琉乃
*/

#include "PlaneProjectionShadow.h"

void PlaneProjectionShadow::Initialize(const std::string& modelName, const WorldTransform* casterWorldTransform)
{
	// 共通処理初期化
	TextureManager::Load("resources/TempTexture/uvChecker.png");
	BaseInstancingObject::Initialize(modelName, "TempTexture/uvChecker.png");

	// 平面投影行列
	shadowMatrix_ = MakeIdentityMatrix();
	shadowMatrix_.m[1][1] = 0.006f;
	casterWorldTransform_ = casterWorldTransform;
	// objectを真っ黒にする
	object_.lock()->color = {0.0f,0.0f,0.0f,1.0f};
  
	if (casterWorldTransform_) {
		Matrix4x4 matWorld = casterWorldTransform_->matWorld * shadowMatrix_;
		//object_->SetMatWorld(matWorld);
	}

}

void PlaneProjectionShadow::Update()
{

	// ワールド行列計算
	if (casterWorldTransform_) {
		Matrix4x4 matWorld = casterWorldTransform_->matWorld * shadowMatrix_;
		//object_->SetMatWorld(matWorld);
	}
}

void PlaneProjectionShadow::Draw(const Camera& camera)
{
	camera;
}
