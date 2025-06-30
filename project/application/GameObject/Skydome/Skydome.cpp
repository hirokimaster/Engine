/**
* @file Skydome.cpp
* @brief 天球生成
* @author 仁平 琉乃
*/

#include "Skydome.h"
#include "engine/Utility/ImGuiManager/ImGuiManager.h"

void Skydome::Initialize()
{
	TextureManager::GetInstance()->Load("resources/skydome/sky2.png");
	object_ = std::make_unique<Object3dPlacer>();
	object_->Initialize();
	object_->SetModel("skydome/skydome.obj");
	Vector3 scale{ 100000.0f,100000.0f,100000.0f };
	object_->SetScale(scale);
	object_->SetTexHandle(TextureManager::GetTexHandle("TempTexture/white.png"));
	material_.color = { 0.1f,0.1f,0.1f,1.0f };
	material_.enableLighting = false;
	object_->SetMaterialProperty(material_);
	uvTransform_ = {
		{3.0f,3.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.5f,8.0f,0.0f}, 
	};
	object_->SetUVTransform(uvTransform_);

}

void Skydome::Update()
{
	object_->Update();
	object_->SetUVTransform(uvTransform_);

}

void Skydome::Draw(const Camera& camera)
{
	object_->Draw(camera);
}
