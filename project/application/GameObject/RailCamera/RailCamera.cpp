
#include "RailCamera.h"

void RailCamera::Initialize(const Vector3& position, const Vector3& rotate)
{
	// ワールドトランスフォーム
	worldTransform_.Initialize();
	worldTransform_.translate = position;
	worldTransform_.rotate = rotate;

	camera_.Initialize();
	camera_.farZ = 2000.0f;
}

void RailCamera::Update()
{
	MoveOnRail(); // 移動

	Rotate();

#ifdef _DEBUG
	// カメラの座標を画面表示する処理
	ImGui::Begin("RailCamera");
	ImGui::SliderFloat3("CameraTranslation ", &worldTransform_.translate.x, -50.0f, 50.0f);
	ImGui::SliderFloat3("CameraRotate ", &worldTransform_.rotate.x, 0.0f, 10.0f);
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

}

void RailCamera::Rotate()
{
	// レティクルの方向ベクトルを求める
	Vector3 reticlePosition = lockOn_->GetWorldPosition3DReticle();
	Vector3 playerToReticle = reticlePosition - worldTransform_.translate;

	// 方向ベクトルがゼロの時は計算しない
	if (Length(playerToReticle) == 0.0f) {
		return;
	}

	// playerの前方ベクトル
	Vector3 forward = { 0.0f, 0.0f, 1.0f };

	// 正規化
	Vector3 playerToReticleNorm = Normalize(playerToReticle);

	// 回転軸と角度を計算
	Vector3 rotationAxis = Cross(forward, playerToReticleNorm);

	if (Length(rotationAxis) == 0.0f) {
		// 軸がゼロベクトルなら回転は不要
		return;
	}

	// 正規化
	rotationAxis = Normalize(rotationAxis);

	float dotProduct = Dot(forward, playerToReticleNorm);
	// -1.0 ～ 1.0の範囲にクランプする
	float clampedDotProduct = std::clamp(dotProduct, -1.0f, 1.0f);
	float angle = std::acos(clampedDotProduct);

	// クォータニオンを作成
	Quaternion rotationQuaternion = MakeRotateAxisAngleQuaternion(rotationAxis, angle);
	Quaternion normalizedQuaternion = QNormalize(rotationQuaternion);

	// 自機の回転を更新
	Vector3 rotateVector = RotateVector(forward, normalizedQuaternion);
	worldTransform_.rotate.x = -rotateVector.y;
	worldTransform_.rotate.y = rotateVector.x;
	worldTransform_.rotate.z = 0.0f;
}

Vector3 RailCamera::CatmullRomInterpolation(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, float t)
{
	Vector3 result{};
	const float s = 0.5f;
	float t2 = t * t;
	float t3 = t2 * t;

	Vector3 e3{};
	e3 = (-1.0f * p0) + (3.0f * p1) - (3.0f * p2) + p3;
	Vector3 e2{};
	e2 = (2.0f * p0) - (5.0f * p1) + (4.0f * p2) - p3;
	Vector3 e1{};
	e1 = p0 + p2;
	Vector3 e0{};
	e0 = 2.0f * p1;

	result = (s * t3 * e3) + (t2 * e2) + (t * e1) + e0;

	return result;
}

Vector3 RailCamera::CatmullRomPosition(const std::vector<Vector3>& points, float t)
{
	assert(points.size() >= 4 && "制御点は4点以上必要です");
	size_t division = points.size() - 1;
	float areaWidth = 1.0f / division;

	float t_2 = std::fmod(t, areaWidth) * division;
	t_2 = std::clamp(t_2, 0.0f, 1.0f);

	size_t index = static_cast<size_t>(t / areaWidth);

	// 上限を超えないようにする
	if (index > points.size() - 2) {
		index = points.size() - 2;
	}

	size_t index0 = index - 1;
	size_t index1 = index;
	size_t index2 = index + 1;
	size_t index3 = index + 2;

	if (index == 0) {
		index0 = index1;
	}

	if (index3 >= points.size()) {
		index3 = index2;
	}


	const Vector3& p0 = points[index0];
	const Vector3& p1 = points[index1];
	const Vector3& p2 = points[index2];
	const Vector3& p3 = points[index3];

	return CatmullRomInterpolation(p0, p1, p2, p3, t_2);
}
