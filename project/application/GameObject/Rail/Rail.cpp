#include "Rail.h"

void Rail::Initialize()
{

	// レールの制御点
	controlPoints_ = {

		{0,0,20},
		{0,0,160},
		{0,0,240},
		{0,0,300},
		{0,0,360},

	};
}

void Rail::Update()
{
	deltaTime_ += 1.0f / 60.0f;

	float progress = ProgressCalc(deltaTime_, 1.0f);

	RailPositionCalc(progress);

}

void Rail::RailPositionCalc(float progress)
{
	float t = progress * (controlPoints_.size() - 3);

	// 現在の位置
	Vector3 railPosition = CatmullRomPosition(controlPoints_, t);

	// playerの位置を合わせる
	player_->SetPositionZ(railPosition.z);
}

Vector3 Rail::CatmullRomInterpolation(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, float t)
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

Vector3 Rail::CatmullRomPosition(const std::vector<Vector3>& points, float t)
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

float Rail::ProgressCalc(float time, float speed)
{
	float dist = time * speed;
	float railLength = 340.0f;
	float progress = dist / railLength;
	// 0.0f ~ 1.0fの範囲にクランプする
	return std::clamp(progress, 0.0f, 1.0f);
}
