#include "RailCamera.h"

void RailCamera::Initialize(const Vector3& position, const Vector3& rotate)
{
	// ワールドトランスフォーム
	worldTransform_.Initialize();
	worldTransform_.translate = position;
	worldTransform_.rotate = rotate;

	camera_.Initialize();
}

void RailCamera::Update()
{
	MoveOnRail(); // 移動

#ifdef _DEBUG
	// カメラの座標を画面表示する処理
	ImGui::Begin("RailCamera");
	ImGui::SliderFloat3("CmeraTranslation ", &worldTransform_.translate.x, -50.0f, 50.0f);
	ImGui::SliderFloat3("CmeraRotate ", &worldTransform_.rotate.x, 0.0f, 10.0f);
	ImGui::End();
#endif // _DEBUG
}

void RailCamera::MoveOnRail()
{
	// 移動
	const float cameraSpeed = 0.1f; // 移動スピード
	velocity_.z = cameraSpeed;
	worldTransform_.translate = worldTransform_.translate + velocity_;

	// ワールドトランスフォームのワールド行列再計算
	worldTransform_.matWorld = MakeAffineMatrix(
		worldTransform_.scale, worldTransform_.rotate, worldTransform_.translate);

	// カメラオブジェクトのワールド行列からビュー行列を計算する
	camera_.matView = Inverse(worldTransform_.matWorld);

	worldTransform_.UpdateMatrix();

}
