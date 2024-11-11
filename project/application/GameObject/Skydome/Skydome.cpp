/**
* @file Skydome.cpp
* @brief 天球生成
* @author 仁平 琉乃
*/

#include "Skydome.h"
#include "engine/Utility/ImGuiManager/ImGuiManager.h"
#include "engine/TextureManager/TextureManager.h"

void Skydome::Initialize()
{

	worldTransform_.Initialize();
	worldTransform_.scale = { 800.0f,800.0f,800.0f };
	texHandle_ = TextureManager::GetInstance()->Load("resources/skydome/sky2.png");

	object_ = std::make_unique<Object3DPlacer>();
	object_->Initialize();
	object_->SetModel("skydome/skydome.obj");
	object_->SetWorldTransform(worldTransform_);
	object_->SetTexHandle(texHandle_);
	material_.color = { 1.0f,1.0f,1.0f,1.0f };
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
	worldTransform_.UpdateMatrix();

	ImGui::Begin("uv");
	ImGui::DragFloat2("scale", &uvTransform_.scale.x, 0.1f, 0.0f, 1000.0f);
	ImGui::DragFloat2("translate", &uvTransform_.translate.x, 0.1f, 0.0f, 1000.0f);
	ImGui::DragFloat3("rotate", &worldTransform_.rotate.x, 0.1f, 0.0f, 100.0f);
	ImGui::End();
	object_->SetUVTransform(uvTransform_);

}

void Skydome::Draw(Camera& camera)
{
	object_->Draw(camera);
}
