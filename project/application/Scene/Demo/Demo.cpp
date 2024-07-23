#include "Demo.h"
#include "engine/ModelManager/ModelManager.h"

Demo::Demo()
{
}

Demo::~Demo()
{
}

void Demo::Initialize()
{
	/*ModelManager::LoadGLTFModel("Walk.gltf");

	postProcess_ = std::make_unique<PostProcess>();
	postProcess_->Initialize();
	postProcess_->SetEffect(Grayscale);
	GameManager::GetInstance()->SetPostProcess(postProcess_.get());

	texHandle_ = TextureManager::Load("resources/taiyou.jpg");
	maskTex_ = TextureManager::Load("resources/noise0.png");
	texHandleUV_ = TextureManager::Load("resources/uvChecker.png");
	sprite_.reset(Sprite::Create(texHandle_));
	object_ = std::make_unique<Object3DPlacer>();
	object_->Initialize();
	object_->SetModel("Walk.gltf");
	object_->SetTexHandle(texHandleUV_);
	trans_.Initialize();
	trans_.scale = { 10.0f,10.0f,10.0f };
	trans_.translate.x = 15.0f;
	trans_.translate.y = -10.0f;
	trans_.rotate.y = std::numbers::pi_v<float>;
	camera_.Initialize();*/
	camera_.Initialize();

	texHandle_ = TextureManager::Load("resources/uvChecker.png");
	loader_ = std::make_unique<Loader>();
	levelData_ = loader_->Load("level");
	loader_->SetTexHandle(texHandle_);
	loader_->Arrangement(levelData_);
}

void Demo::Update()
{
	loader_->UpdateCamera();
	camera_ = loader_->GetCamera();
	camera_.TransferMatrix();

	//static int currentItem_ = 0;
	//const char* item[9] = { "Grayscale", "Vignette", "Bloom","Gaussian","LuminanceOutline",
	//	"DepthOutline","RadialBlur","Dissolve","Random" };

	//ImGui::Begin("Effect");
	//if (ImGui::Combo("Type", &currentItem_, item, IM_ARRAYSIZE(item))) {
	//	if (currentItem_ == 0) {
	//		postProcess_->SetEffect(Grayscale);
	//	}
	//	else if (currentItem_ == 1) {
	//		postProcess_->SetEffect(Vignette);
	//	}
	//	else if (currentItem_ == 2) {
	//		postProcess_->SetEffect(Bloom);
	//	}
	//	else if (currentItem_ == 3) {
	//		postProcess_->SetEffect(GaussianBlur);
	//	}
	//	else if (currentItem_ == 4) {
	//		postProcess_->SetEffect(LuminanceOutline);
	//	}
	//	else if (currentItem_ == 5) {
	//		postProcess_->SetEffect(DepthOutline);
	//	}
	//	else if (currentItem_ == 6) {
	//		postProcess_->SetEffect(RadialBlur);
	//	}
	//	else if (currentItem_ == 7) {
	//		postProcess_->SetEffect(Dissolve);
	//		postProcess_->SetMaskTexture(maskTex_);

	//	}
	//	else if (currentItem_ == 8) {
	//		postProcess_->SetEffect(Random);
	//	}

	//}

	////	ImGui::DragFloat("RandomTime", &time_, 0.1f, 0.0f, 100.0f);

	//ImGui::End();

	//postProcess_->SetRandomParam(param_);
	//param_.time += 1.0f / 120.0f;


	//postProcess_->SetDissolveParam(DissolvePram_);

	//if (DissolvePram_.threshold >= 1.0f) {
	//	dFlag_2 = true;
	//	dFlag_1 = false;
	//}

	//if (DissolvePram_.threshold <= 0.0f) {
	//	dFlag_1 = true;
	//	dFlag_2 = false;
	//}

	//if (dFlag_1) {
	//	DissolvePram_.threshold += 0.01f;
	//}

	//if (dFlag_2) {
	//	DissolvePram_.threshold -= 0.01f;
	//}



	//trans_.UpdateMatrix();
	//camera_.UpdateMatrix();
}

void Demo::Draw()
{
	loader_->Draw(camera_);
	//postProcess_->Draw();
}

void Demo::PostProcessDraw()
{
	/*postProcess_->PreDraw();

	object_->Draw(trans_, camera_);
	sprite_->Draw();


	postProcess_->PostDraw();*/
}
