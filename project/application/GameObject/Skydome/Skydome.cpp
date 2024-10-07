
#include "Skydome.h"
#include "engine/Utility/ImGuiManager/ImGuiManager.h"
#include "engine/TextureManager/TextureManager.h"

void Skydome::Initialize()
{

	worldTransform_.Initialize();
	worldTransform_.scale = { 300.0f,300.0f,300.0f };
	texHandle_ = TextureManager::GetInstance()->Load("resources/skydome/sky2.png");

	object_ = std::make_unique<Object3DPlacer>();
	object_->Initialize();
	object_->SetModel("skydome/skydome.obj");
	object_->SetWorldTransform(worldTransform_);
	object_->SetTexHandle(texHandle_);
	material_.color = { 1.0f,1.0f,1.0f,1.0f };
	material_.enableLighting = false;
	object_->SetMaterialProperty(material_);

}

void Skydome::Update()
{
	worldTransform_.UpdateMatrix();

}

void Skydome::Draw(Camera& camera)
{
	object_->Draw(camera);
}
