#include "Mathfunction.h"
#include <cmath>

// 行列の積
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result;
	result.m[0][0] = (m1.m[0][0] * m2.m[0][0]) + (m1.m[0][1] * m2.m[1][0]) +
		(m1.m[0][2] * m2.m[2][0]) + (m1.m[0][3] * m2.m[3][0]);
	result.m[0][1] = (m1.m[0][0] * m2.m[0][1]) + (m1.m[0][1] * m2.m[1][1]) +
		(m1.m[0][2] * m2.m[2][1]) + (m1.m[0][3] * m2.m[3][1]);
	result.m[0][2] = (m1.m[0][0] * m2.m[0][2]) + (m1.m[0][1] * m2.m[1][2]) +
		(m1.m[0][2] * m2.m[2][2]) + (m1.m[0][3] * m2.m[3][2]);
	result.m[0][3] = (m1.m[0][0] * m2.m[0][3]) + (m1.m[0][1] * m2.m[1][3]) +
		(m1.m[0][2] * m2.m[2][3]) + (m1.m[0][3] * m2.m[3][3]);
	result.m[1][0] = (m1.m[1][0] * m2.m[0][0]) + (m1.m[1][1] * m2.m[1][0]) +
		(m1.m[1][2] * m2.m[2][0]) + (m1.m[1][3] * m2.m[3][0]);
	result.m[1][1] = (m1.m[1][0] * m2.m[0][1]) + (m1.m[1][1] * m2.m[1][1]) +
		(m1.m[1][2] * m2.m[2][1]) + (m1.m[1][3] * m2.m[3][1]);
	result.m[1][2] = (m1.m[1][0] * m2.m[0][2]) + (m1.m[1][1] * m2.m[1][2]) +
		(m1.m[1][2] * m2.m[2][2]) + (m1.m[1][3] * m2.m[3][2]);
	result.m[1][3] = (m1.m[1][0] * m2.m[0][3]) + (m1.m[1][1] * m2.m[1][3]) +
		(m1.m[1][2] * m2.m[2][3]) + (m1.m[1][3] * m2.m[3][3]);

	result.m[2][0] = (m1.m[2][0] * m2.m[0][0]) + (m1.m[2][1] * m2.m[1][0]) +
		(m1.m[2][2] * m2.m[2][0]) + (m1.m[2][3] * m2.m[3][0]);
	result.m[2][1] = (m1.m[2][0] * m2.m[0][1]) + (m1.m[2][1] * m2.m[1][1]) +
		(m1.m[2][2] * m2.m[2][1]) + (m1.m[2][3] * m2.m[3][1]);
	result.m[2][2] = (m1.m[2][0] * m2.m[0][2]) + (m1.m[2][1] * m2.m[1][2]) +
		(m1.m[2][2] * m2.m[2][2]) + (m1.m[2][3] * m2.m[3][2]);
	result.m[2][3] = (m1.m[2][0] * m2.m[0][3]) + (m1.m[2][1] * m2.m[1][3]) +
		(m1.m[2][2] * m2.m[2][3]) + (m1.m[2][3] * m2.m[3][3]);
	result.m[3][0] = (m1.m[3][0] * m2.m[0][0]) + (m1.m[3][1] * m2.m[1][0]) +
		(m1.m[3][2] * m2.m[2][0]) + (m1.m[3][3] * m2.m[3][0]);
	result.m[3][1] = (m1.m[3][0] * m2.m[0][1]) + (m1.m[3][1] * m2.m[1][1]) +
		(m1.m[3][2] * m2.m[2][1]) + (m1.m[3][3] * m2.m[3][1]);
	result.m[3][2] = (m1.m[3][0] * m2.m[0][2]) + (m1.m[3][1] * m2.m[1][2]) +
		(m1.m[3][2] * m2.m[2][2]) + (m1.m[3][3] * m2.m[3][2]);
	result.m[3][3] = (m1.m[3][0] * m2.m[0][3]) + (m1.m[3][1] * m2.m[1][3]) +
		(m1.m[3][2] * m2.m[2][3]) + (m1.m[3][3] * m2.m[3][3]);

	return result;
}

// 1. 行列の加法
Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result;
	result.m[0][0] = m1.m[0][0] + m2.m[0][0];
	result.m[0][1] = m1.m[0][1] + m2.m[0][1];
	result.m[0][2] = m1.m[0][2] + m2.m[0][2];
	result.m[0][3] = m1.m[0][3] + m2.m[0][3];
	result.m[1][0] = m1.m[1][0] + m2.m[1][0];
	result.m[1][1] = m1.m[1][1] + m2.m[1][1];
	result.m[1][2] = m1.m[1][2] + m2.m[1][2];
	result.m[1][3] = m1.m[1][3] + m2.m[1][3];
	result.m[2][0] = m1.m[2][0] + m2.m[2][0];
	result.m[2][1] = m1.m[2][1] + m2.m[2][1];
	result.m[2][2] = m1.m[2][2] + m2.m[2][2];
	result.m[2][3] = m1.m[2][3] + m2.m[2][3];
	result.m[3][0] = m1.m[3][0] + m2.m[3][0];
	result.m[3][1] = m1.m[3][1] + m2.m[3][1];
	result.m[3][2] = m1.m[3][2] + m2.m[3][2];
	result.m[3][3] = m1.m[3][3] + m2.m[3][3];
	return result;
}

// 2. 行列の減法
Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result;
	result.m[0][0] = m1.m[0][0] - m2.m[0][0];
	result.m[0][1] = m1.m[0][1] - m2.m[0][1];
	result.m[0][2] = m1.m[0][2] - m2.m[0][2];
	result.m[0][3] = m1.m[0][3] - m2.m[0][3];
	result.m[1][0] = m1.m[1][0] - m2.m[1][0];
	result.m[1][1] = m1.m[1][1] - m2.m[1][1];
	result.m[1][2] = m1.m[1][2] - m2.m[1][2];
	result.m[1][3] = m1.m[1][3] - m2.m[1][3];
	result.m[2][0] = m1.m[2][0] - m2.m[2][0];
	result.m[2][1] = m1.m[2][1] - m2.m[2][1];
	result.m[2][2] = m1.m[2][2] - m2.m[2][2];
	result.m[2][3] = m1.m[2][3] - m2.m[2][3];
	result.m[3][0] = m1.m[3][0] - m2.m[3][0];
	result.m[3][1] = m1.m[3][1] - m2.m[3][1];
	result.m[3][2] = m1.m[3][2] - m2.m[3][2];
	result.m[3][3] = m1.m[3][3] - m2.m[3][3];
	return result;
}

// 拡大縮小行列
Matrix4x4 MakeScaleMatrix(const Vector3& scale) {
	Matrix4x4 result;

	result.m[0][0] = scale.x;
	result.m[0][1] = 0.0f;
	result.m[0][2] = 0.0f;
	result.m[0][3] = 0.0f;
	result.m[1][0] = 0.0f;
	result.m[1][1] = scale.y;
	result.m[1][2] = 0.0f;
	result.m[1][3] = 0.0f;
	result.m[2][0] = 0.0f;
	result.m[2][1] = 0.0f;
	result.m[2][2] = scale.z;
	result.m[2][3] = 0.0f;
	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;

	return result;
}

// 平行移動
Matrix4x4 MakeTranslateMatrix(const Vector3& translate) {
	Matrix4x4 result;

	result.m[0][0] = 1.0f;
	result.m[0][1] = 0.0f;
	result.m[0][2] = 0.0f;
	result.m[0][3] = 0.0f;
	result.m[1][0] = 0.0f;
	result.m[1][1] = 1.0f;
	result.m[1][2] = 0.0f;
	result.m[1][3] = 0.0f;
	result.m[2][0] = 0.0f;
	result.m[2][1] = 0.0f;
	result.m[2][2] = 1.0f;
	result.m[2][3] = 0.0f;
	result.m[3][0] = translate.x;
	result.m[3][1] = translate.y;
	result.m[3][2] = translate.z;
	result.m[3][3] = 1.0f;

	return result;
}

/*---------------------------------
 回転行列
------------------------------------*/

// X軸
Matrix4x4 MakeRotateXMatrix(float radian) {
	Matrix4x4 result;
	float cos = std::cos(radian);
	float sin = std::sin(radian);

	result.m[0][0] = 1.0f;
	result.m[0][1] = 0.0f;
	result.m[0][2] = 0.0f;
	result.m[0][3] = 0.0f;
	result.m[1][0] = 0.0f;
	result.m[1][1] = cos;
	result.m[1][2] = sin;
	result.m[1][3] = 0.0f;
	result.m[2][0] = 0.0f;
	result.m[2][1] = -sin;
	result.m[2][2] = cos;
	result.m[2][3] = 0.0f;
	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;

	return result;
}

// Y軸
Matrix4x4 MakeRotateYMatrix(float radian) {
	Matrix4x4 result;
	float cos = std::cos(radian);
	float sin = std::sin(radian);

	result.m[0][0] = cos;
	result.m[0][1] = 0.0f;
	result.m[0][2] = -sin;
	result.m[0][3] = 0.0f;
	result.m[1][0] = 0.0f;
	result.m[1][1] = 1.0f;
	result.m[1][2] = 0.0f;
	result.m[1][3] = 0.0f;
	result.m[2][0] = sin;
	result.m[2][1] = 0.0f;
	result.m[2][2] = cos;
	result.m[2][3] = 0.0f;
	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;

	return result;
}

// Z軸
Matrix4x4 MakeRotateZMatrix(float radian) {
	Matrix4x4 result;
	float cos = std::cos(radian);
	float sin = std::sin(radian);

	result.m[0][0] = cos;
	result.m[0][1] = sin;
	result.m[0][2] = 0.0f;
	result.m[0][3] = 0.0f;
	result.m[1][0] = -sin;
	result.m[1][1] = cos;
	result.m[1][2] = 0.0f;
	result.m[1][3] = 0.0f;
	result.m[2][0] = 0.0f;
	result.m[2][1] = 0.0f;
	result.m[2][2] = 1.0f;
	result.m[2][3] = 0.0f;
	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;

	return result;
}

// アフィン変換
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& translate) {

	// スケーリング行列
	Matrix4x4 Scale = MakeScaleMatrix(scale);

	// 回転行列
	Matrix4x4 RotateX = MakeRotateXMatrix(rot.x);
	Matrix4x4 RotateY = MakeRotateYMatrix(rot.y);
	Matrix4x4 RotateZ = MakeRotateZMatrix(rot.z);

	// 回転行列合成
	Matrix4x4 Rotate = Multiply(Multiply(RotateX, RotateY), RotateZ);

	// 平行移動行列
	Matrix4x4 Translate = MakeTranslateMatrix(translate);

	// スケール、回転、平行移動の合成
	Matrix4x4 Transform = Multiply(Multiply(Scale, Rotate), Translate);

	return Transform;
}

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Quaternion& rotate, const Vector3& translate)
{
	// スケーリング行列
	Matrix4x4 Scale = MakeScaleMatrix(scale);

	Matrix4x4 Rotate = MakeRotateMatrix(rotate);

	// 平行移動行列
	Matrix4x4 Translate = MakeTranslateMatrix(translate);

	// スケール、回転、平行移動の合成
	Matrix4x4 Transform = Scale * Rotate * Translate;

	return Transform;
}

// ビルボード用のワールド行列
Matrix4x4 MakeBiilboardWorldMatrix(const Vector3& scale, const Matrix4x4& billboard, const Vector3& translate) {
	// スケーリング行列
	Matrix4x4 Scale = MakeScaleMatrix(scale);
	// 平行移動行列
	Matrix4x4 Translate = MakeTranslateMatrix(translate);
	// ワールド行列
	Matrix4x4 worldMatrix = Multiply(Scale, Multiply(billboard, Translate));

	return worldMatrix;

}

// 透視投影行列
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip) {
	Matrix4x4 result;
	float cot = 1.0f / std::tan(fovY / 2.0f);

	result.m[0][0] = (1.0f / aspectRatio) * cot;
	result.m[0][1] = 0.0f;
	result.m[0][2] = 0.0f;
	result.m[0][3] = 0.0f;
	result.m[1][0] = 0.0f;
	result.m[1][1] = cot;
	result.m[1][2] = 0.0f;
	result.m[1][3] = 0.0f;
	result.m[2][0] = 0.0f;
	result.m[2][1] = 0.0f;
	result.m[2][2] = farClip / (farClip - nearClip);
	result.m[2][3] = 1.0f;
	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = ( - nearClip * farClip) / (farClip - nearClip);
	result.m[3][3] = 0.0f;

	return result;
}

// 正射影行列
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip) {
	Matrix4x4 result;

	result.m[0][0] = 2.0f / (right - left);
	result.m[0][1] = 0.0f;
	result.m[0][2] = 0.0f;
	result.m[0][3] = 0.0f;
	result.m[1][0] = 0.0f;
	result.m[1][1] = 2.0f / (top - bottom);
	result.m[1][2] = 0.0f;
	result.m[1][3] = 0.0f;
	result.m[2][0] = 0.0f;
	result.m[2][1] = 0.0f;
	result.m[2][2] = 1.0f / (farClip - nearClip);
	result.m[2][3] = 0.0f;
	result.m[3][0] = (left + right) / (left - right);
	result.m[3][1] = (top + bottom) / (bottom - top);
	result.m[3][2] = nearClip / (nearClip - farClip);
	result.m[3][3] = 1.0f;

	return result;
}

// 逆行列
Matrix4x4 Inverse(const Matrix4x4& m) {
	Matrix4x4 result;

	// 4x4の行列式を求める
	float determinant = (m.m[0][0] * m.m[1][1] * m.m[2][2] * m.m[3][3]) + (m.m[0][0] * m.m[1][2] * m.m[2][3] * m.m[3][1]) + (m.m[0][0] * m.m[1][3] * m.m[2][1] * m.m[3][2])
		- (m.m[0][0] * m.m[1][3] * m.m[2][2] * m.m[3][1]) - (m.m[0][0] * m.m[1][2] * m.m[2][1] * m.m[3][3]) - (m.m[0][0] * m.m[1][1] * m.m[2][3] * m.m[3][2])
		- (m.m[0][1] * m.m[1][0] * m.m[2][2] * m.m[3][3]) - (m.m[0][2] * m.m[1][0] * m.m[2][3] * m.m[3][1]) - (m.m[0][3] * m.m[1][0] * m.m[2][1] * m.m[3][2])
		+ (m.m[0][3] * m.m[1][0] * m.m[2][2] * m.m[3][1]) + (m.m[0][2] * m.m[1][0] * m.m[2][1] * m.m[3][3]) + (m.m[0][1] * m.m[1][0] * m.m[2][3] * m.m[3][2])
		+ (m.m[0][1] * m.m[1][2] * m.m[2][0] * m.m[3][3]) + (m.m[0][2] * m.m[1][3] * m.m[2][0] * m.m[3][1]) + (m.m[0][3] * m.m[1][1] * m.m[2][0] * m.m[3][2])
		- (m.m[0][3] * m.m[1][2] * m.m[2][0] * m.m[3][1]) - (m.m[0][2] * m.m[1][1] * m.m[2][0] * m.m[3][3]) - (m.m[0][1] * m.m[1][3] * m.m[2][0] * m.m[3][2])
		- (m.m[0][1] * m.m[1][2] * m.m[2][3] * m.m[3][0]) - (m.m[0][2] * m.m[1][3] * m.m[2][1] * m.m[3][0]) - (m.m[0][3] * m.m[1][1] * m.m[2][2] * m.m[3][0])
		+ (m.m[0][3] * m.m[1][2] * m.m[2][1] * m.m[3][0]) + (m.m[0][2] * m.m[1][1] * m.m[2][3] * m.m[3][0]) + (m.m[0][1] * m.m[1][3] * m.m[2][2] * m.m[3][0]);

	float a = 1.0f / determinant; // 1÷行列式(1/|A|)

	// 逆行列を求める
	result.m[0][0] = a * ((m.m[1][1] * m.m[2][2] * m.m[3][3]) + (m.m[1][2] * m.m[2][3] * m.m[3][1]) + (m.m[1][3] * m.m[2][1] * m.m[3][2])
		- (m.m[1][3] * m.m[2][2] * m.m[3][1]) - (m.m[1][2] * m.m[2][1] * m.m[3][3]) - (m.m[1][1] * m.m[2][3] * m.m[3][2]));

	result.m[0][1] = a * (-(m.m[0][1] * m.m[2][2] * m.m[3][3]) - (m.m[0][2] * m.m[2][3] * m.m[3][1]) - (m.m[0][3] * m.m[2][1] * m.m[3][2])
		+ (m.m[0][3] * m.m[2][2] * m.m[3][1]) + (m.m[0][2] * m.m[2][1] * m.m[3][3]) + (m.m[0][1] * m.m[2][3] * m.m[3][2]));

	result.m[0][2] = a * ((m.m[0][1] * m.m[1][2] * m.m[3][3]) + (m.m[0][2] * m.m[1][3] * m.m[3][1]) + (m.m[0][3] * m.m[1][1] * m.m[3][2])
		- (m.m[0][3] * m.m[1][2] * m.m[3][1]) - (m.m[0][2] * m.m[1][1] * m.m[3][3]) - (m.m[0][1] * m.m[1][3] * m.m[3][2]));

	result.m[0][3] = a * (-(m.m[0][1] * m.m[1][2] * m.m[2][3]) - (m.m[0][2] * m.m[1][3] * m.m[2][1]) - (m.m[0][3] * m.m[1][1] * m.m[2][2])
		+ (m.m[0][3] * m.m[1][2] * m.m[2][1]) + (m.m[0][2] * m.m[1][1] * m.m[2][3]) + (m.m[0][1] * m.m[1][3] * m.m[2][2]));

	result.m[1][0] = a * (-(m.m[1][0] * m.m[2][2] * m.m[3][3]) - (m.m[1][2] * m.m[2][3] * m.m[3][0]) - (m.m[1][3] * m.m[2][0] * m.m[3][2])
		+ (m.m[1][3] * m.m[2][2] * m.m[3][0]) + (m.m[1][2] * m.m[2][0] * m.m[3][3]) + (m.m[1][0] * m.m[2][3] * m.m[3][2]));

	result.m[1][1] = a * ((m.m[0][0] * m.m[2][2] * m.m[3][3]) + (m.m[0][2] * m.m[2][3] * m.m[3][0]) + (m.m[0][3] * m.m[2][0] * m.m[3][2])
		- (m.m[0][3] * m.m[2][2] * m.m[3][0]) - (m.m[0][2] * m.m[2][0] * m.m[3][3]) - (m.m[0][0] * m.m[2][3] * m.m[3][2]));

	result.m[1][2] = a * (-(m.m[0][0] * m.m[1][2] * m.m[3][3]) - (m.m[0][2] * m.m[1][3] * m.m[3][0]) - (m.m[0][3] * m.m[1][0] * m.m[3][2])
		+ (m.m[0][3] * m.m[1][2] * m.m[3][0]) + (m.m[0][2] * m.m[1][0] * m.m[3][3]) + (m.m[0][0] * m.m[1][3] * m.m[3][2]));

	result.m[1][3] = a * ((m.m[0][0] * m.m[1][2] * m.m[2][3]) + (m.m[0][2] * m.m[1][3] * m.m[2][0]) + (m.m[0][3] * m.m[1][0] * m.m[2][2])
		- (m.m[0][3] * m.m[1][2] * m.m[2][0]) - (m.m[0][2] * m.m[1][0] * m.m[2][3]) - (m.m[0][0] * m.m[1][3] * m.m[2][2]));

	result.m[2][0] = a * ((m.m[1][0] * m.m[2][1] * m.m[3][3]) + (m.m[1][1] * m.m[2][3] * m.m[3][0]) + (m.m[1][3] * m.m[2][0] * m.m[3][1])
		- (m.m[1][3] * m.m[2][1] * m.m[3][0]) - (m.m[1][1] * m.m[2][0] * m.m[3][3]) - (m.m[1][0] * m.m[2][3] * m.m[3][1]));

	result.m[2][1] = a * (-(m.m[0][0] * m.m[2][1] * m.m[3][3]) - (m.m[0][1] * m.m[2][3] * m.m[3][0]) - (m.m[0][3] * m.m[2][0] * m.m[3][1])
		+ (m.m[0][3] * m.m[2][1] * m.m[3][0]) + (m.m[0][1] * m.m[2][0] * m.m[3][3]) + (m.m[0][0] * m.m[2][3] * m.m[3][1]));

	result.m[2][2] = a * ((m.m[0][0] * m.m[1][1] * m.m[3][3]) + (m.m[0][1] * m.m[1][3] * m.m[3][0]) + (m.m[0][3] * m.m[1][0] * m.m[3][1])
		- (m.m[0][3] * m.m[1][1] * m.m[2][0]) - (m.m[0][1] * m.m[1][0] * m.m[3][3]) - (m.m[0][0] * m.m[1][3] * m.m[3][1]));

	result.m[2][3] = a * (-(m.m[0][0] * m.m[1][1] * m.m[2][3]) - (m.m[0][1] * m.m[1][3] * m.m[2][0]) - (m.m[0][3] * m.m[1][0] * m.m[2][1])
		+ (m.m[0][3] * m.m[1][1] * m.m[2][0]) + (m.m[0][1] * m.m[1][0] * m.m[2][3]) + (m.m[0][0] * m.m[1][3] * m.m[2][1]));

	result.m[3][0] = a * (-(m.m[1][0] * m.m[2][1] * m.m[3][2]) - (m.m[1][1] * m.m[2][2] * m.m[3][0]) - (m.m[1][2] * m.m[2][0] * m.m[3][1])
		+ (m.m[1][2] * m.m[2][1] * m.m[3][0]) + (m.m[1][1] * m.m[2][0] * m.m[3][2]) + (m.m[1][0] * m.m[2][2] * m.m[3][1]));

	result.m[3][1] = a * ((m.m[0][0] * m.m[2][1] * m.m[3][2]) + (m.m[0][1] * m.m[2][2] * m.m[3][0]) + (m.m[0][2] * m.m[2][0] * m.m[3][1])
		- (m.m[0][2] * m.m[2][1] * m.m[3][0]) - (m.m[0][1] * m.m[2][0] * m.m[3][2]) - (m.m[0][0] * m.m[2][2] * m.m[3][1]));

	result.m[3][2] = a * (-(m.m[0][0] * m.m[1][1] * m.m[3][2]) - (m.m[0][1] * m.m[1][2] * m.m[3][0]) - (m.m[0][2] * m.m[1][0] * m.m[3][1])
		+ (m.m[0][2] * m.m[1][1] * m.m[3][0]) + (m.m[0][1] * m.m[1][0] * m.m[3][2]) + (m.m[0][0] * m.m[1][2] * m.m[3][1]));

	result.m[3][3] = a * ((m.m[0][0] * m.m[1][1] * m.m[2][2]) + (m.m[0][1] * m.m[1][2] * m.m[2][0]) + (m.m[0][2] * m.m[1][0] * m.m[2][1])
		- (m.m[0][2] * m.m[1][1] * m.m[2][0]) - (m.m[0][1] * m.m[1][0] * m.m[2][2]) - (m.m[0][0] * m.m[1][2] * m.m[2][1]));

	return result;

}

// 単位行列
Matrix4x4 MakeIdentityMatrix() {
	static const Matrix4x4 result{ 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
								  0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };

	return result;
}

Matrix4x4 InverseTranspose(const Matrix4x4& m)
{
	Matrix4x4 result;
	result = Transpose(m);
	result = Inverse(result);

	return result;
}

Matrix4x4 Transpose(const Matrix4x4& m)
{
	Matrix4x4 result;

	result.m[1][0] = m.m[0][1];
	result.m[1][2] = m.m[2][1];
	result.m[1][3] = m.m[3][1];
	result.m[2][0] = m.m[0][2];
	result.m[2][1] = m.m[1][2];
	result.m[2][3] = m.m[3][2];
	result.m[3][0] = m.m[0][3];
	result.m[3][1] = m.m[1][3];
	result.m[3][2] = m.m[2][3];


	return result;
}

//正規化
Vector3 Normalize(const Vector3& v) {
	Vector3 result;
	float norm = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
	result.x = v.x / norm;
	result.y = v.y / norm;
	result.z = v.z / norm;
	return result;
}

// 変換
Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix) {
	Vector3 result;
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] +
		1.0f * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] +
		1.0f * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] +
		1.0f * matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] +
		1.0f * matrix.m[3][3];
	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	result.z /= w;

	return result;
}

// 長さ（ノルム）
float Length(const Vector3& v) {
	float result;
	result = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
	return result;
}

// 加算
Vector3 Add(const Vector3& v1, const Vector3& v2) {
	Vector3 result;
	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;
	result.z = v1.z + v2.z;
	return result;
}

// 減算
Vector3 Subtract(const Vector3& v1, const Vector3& v2) {
	Vector3 result;
	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	result.z = v1.z - v2.z;
	return result;
}

// スカラー倍
Vector3 Multiply(float scalar, const Vector3& v) {
	Vector3 result;
	result.x = scalar * v.x;
	result.y = scalar * v.y;
	result.z = scalar * v.z;
	return result;
}

// クロス積
Vector3 Cross(const Vector3& v1, const Vector3& v2) {
	Vector3 result;
	result.x = v1.y * v2.z - v1.z * v2.y;
	result.y = v1.z * v2.x - v1.x * v2.z;
	result.z = v1.x * v2.y - v1.y * v2.x;
	return result;
}

// 内積
float Dot(const Vector3& v1, const Vector3& v2) {
	float result;
	result = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	return result;
}

Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t) {
	Vector3 p;
	p.x = (1.0f - t) * v1.x + t * v2.x;
	p.y = (1.0f - t) * v1.y + t * v2.y;
	p.z = (1.0f - t) * v1.z + t * v2.z;
	return p;
};




Vector3 SLerp(const Vector3& v1, const Vector3& v2, float t) {
	Vector3 p;

	Vector3 s;
	Vector3 e;

	s = Normalize(v1);
	e = Normalize(v2);
	float angle = acos(Dot(s, e));
	// SinΘ
	float SinTh = sin(angle);

	// 補間係数
	float Ps = sin(angle * (1 - t));
	float Pe = sin(angle * t);

	p.x = (Ps * s.x + Pe * e.x) / SinTh;
	p.y = (Ps * s.y + Pe * e.y) / SinTh;
	p.z = (Ps * s.z + Pe * e.z) / SinTh;

	p = Normalize(p);


	return p;
};

Matrix4x4 DirectionToDirection(const Vector3& from, const Vector3& to)
{

	Vector3 axis = Cross(from, to);

	// 0除算してたら
	if (axis.x == 0 && axis.y == 0 && axis.z == 0) {
		axis.x = from.y;
		axis.y = -from.x;
		axis.z = 0.0f;
	}

	axis = Normalize(axis);
	Vector3 uv = Cross(from, to);
	float sin = Length(uv);
	float cos = Dot(from, to);

	Matrix4x4 result;
	Matrix4x4 S;
	Matrix4x4 P;
	Matrix4x4 C;

	S.m[0][0] = cos;
	S.m[0][1] = 0.0f;
	S.m[0][2] = 0.0f;
	S.m[0][3] = 0.0f;
	S.m[1][0] = 0.0f;
	S.m[1][1] = cos;
	S.m[1][2] = 0.0f;
	S.m[1][3] = 0.0f;
	S.m[2][0] = 0.0f;
	S.m[2][1] = 0.0f;
	S.m[2][2] = cos;
	S.m[2][3] = 0.0f;
	S.m[3][0] = 0.0f;
	S.m[3][1] = 0.0f;
	S.m[3][2] = 0.0f;
	S.m[3][3] = 1.0f;

	P.m[0][0] = axis.x * axis.x * (1.0f - cos);
	P.m[0][1] = axis.x * axis.y * (1.0f - cos);
	P.m[0][2] = axis.x * axis.z * (1.0f - cos);
	P.m[0][3] = 0.0f;
	P.m[1][0] = axis.x * axis.y * (1.0f - cos);
	P.m[1][1] = axis.y * axis.y * (1.0f - cos);
	P.m[1][2] = axis.y * axis.z * (1.0f - cos);
	P.m[1][3] = 0.0f;
	P.m[2][0] = axis.x * axis.z * (1.0f - cos);
	P.m[2][1] = axis.y * axis.z * (1.0f - cos);
	P.m[2][2] = axis.z * axis.z * (1.0f - cos);
	P.m[2][3] = 0.0f;
	P.m[3][0] = 0.0f;
	P.m[3][1] = 0.0f;
	P.m[3][2] = 0.0f;
	P.m[3][3] = 1.0f;

	C.m[0][0] = 0.0f;
	C.m[0][1] = -axis.z * sin;
	C.m[0][2] = axis.y * sin;
	C.m[0][3] = 0.0f;
	C.m[1][0] = axis.z * sin;
	C.m[1][1] = 0.0f;
	C.m[1][2] = -axis.x * sin;
	C.m[1][3] = 0.0f;
	C.m[2][0] = -axis.y * sin;
	C.m[2][1] = axis.x * sin;
	C.m[2][2] = 0.0f;
	C.m[2][3] = 0.0f;
	C.m[3][0] = 0.0f;
	C.m[3][1] = 0.0f;
	C.m[3][2] = 0.0f;
	C.m[3][3] = 1.0f;

	result = Add(S, Subtract(P, C));

	return result;
}

//
Quaternion Multiply(const Quaternion& lhs, const Quaternion& rhs)
{
	Quaternion result{};
	result.w = lhs.w * rhs.w - lhs.x * rhs.x - rhs.y * lhs.y - lhs.z * rhs.z;
	result.x = lhs.y * rhs.z - lhs.z * rhs.y + rhs.w * lhs.x + lhs.w * rhs.x;
	result.y = lhs.z * rhs.x - lhs.x * rhs.z + rhs.w * lhs.y + lhs.w * rhs.y;
	result.z = lhs.x * rhs.y - lhs.y * rhs.x + rhs.w * lhs.z + lhs.w * rhs.z;
	return result;
}

Quaternion IdentityQuaternion()
{
	Quaternion result{};
	result.w = 1.0f;
	result.x = 0.0f;
	result.y = 0.0f;
	result.z = 0.0f;
	return result;
}

Quaternion Conjugate(const Quaternion& quaternion)
{
	Quaternion result{};
	result.w = 1.0f;
	result.x = quaternion.x * -1.0f;
	result.y = quaternion.y * -1.0f;
	result.z = quaternion.z * -1.0f;
	return result;
}

float Norm(const Quaternion& quaternion)
{
	float result{};
	result = sqrtf(quaternion.x * quaternion.x +
		quaternion.y * quaternion.y +
		quaternion.z * quaternion.z +
		quaternion.w * quaternion.w);
	return result;
}

Quaternion QNormalize(const Quaternion& quaternion)
{
	Quaternion result{};
	result.w = quaternion.w / Norm(quaternion);
	result.x = quaternion.x / Norm(quaternion);
	result.y = quaternion.y / Norm(quaternion);
	result.z = quaternion.z / Norm(quaternion);
	return result;
}

Quaternion QInverse(const Quaternion& quaternion)
{
	Quaternion result{};
	Quaternion q{};
	float n = Norm(quaternion) * Norm(quaternion);
	q = Conjugate(quaternion);
	result.w = q.w / n;
	result.x = q.x / n;
	result.y = q.y / n;
	result.z = q.z / n;

	return result;
}

Quaternion MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle)
{
	Quaternion result{};
	float cos = std::cos(angle / 2.0f);
	float sin = std::sin(angle / 2.0f);
	Vector3 n = Normalize(axis);

	result.x = sin * n.x;
	result.y = sin * n.y;
	result.z = sin * n.z;
	result.w = cos;

	return result;
}

Vector3 RotateVector(const Vector3& vector, const Quaternion& quaternion)
{
	Vector3 result{};
	Quaternion r{};
	Quaternion q2{};
	r.x = vector.x;
	r.y = vector.y;
	r.z = vector.z;
	r.w = 0.0f;
	q2 = Multiply(Multiply(quaternion, r), Conjugate(quaternion));
	result.x = q2.x;
	result.y = q2.y;
	result.z = q2.z;

	return result;
}

Matrix4x4 MakeRotateMatrix(const Quaternion& quaternion)
{
	Matrix4x4 result{};

	result.m[0][0] = (quaternion.w * quaternion.w) + (quaternion.x * quaternion.x)
		- (quaternion.y * quaternion.y) - (quaternion.z * quaternion.z);
	result.m[0][1] = 2.0f * ((quaternion.x * quaternion.y) + (quaternion.w * quaternion.z));
	result.m[0][2] = 2.0f * ((quaternion.x * quaternion.z) - (quaternion.w * quaternion.y));
	result.m[0][3] = 0.0f;
	result.m[1][0] = 2.0f * ((quaternion.x * quaternion.y) - (quaternion.w * quaternion.z));
	result.m[1][1] = (quaternion.w * quaternion.w) - (quaternion.x * quaternion.x)
		+ (quaternion.y * quaternion.y) - (quaternion.z * quaternion.z);
	result.m[1][2] = 2.0f * ((quaternion.y * quaternion.z) + (quaternion.w * quaternion.x));
	result.m[1][3] = 0.0f;
	result.m[2][0] = 2.0f * ((quaternion.x * quaternion.z) + (quaternion.w * quaternion.y));
	result.m[2][1] = 2.0f * ((quaternion.y * quaternion.z) - (quaternion.w * quaternion.x));
	result.m[2][2] = (quaternion.w * quaternion.w) - (quaternion.x * quaternion.x)
		- (quaternion.y * quaternion.y) + (quaternion.z * quaternion.z);
	result.m[2][3] = 0.0f;
	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;

	return result;
}

Quaternion Slerp(const Quaternion& q0, const Quaternion& q1, float t)
{
	Quaternion result{};
	Vector3 q0Vec = { q0.x,q0.y,q0.z };
	Vector3 q1Vec = { q1.x,q1.y,q1.z };
	float q0W = q0.w;
	float dot = Dot(q0Vec, q1Vec) + q0.w * q1.w;
	if (dot < 0) {
		q0Vec.x = -1.0f * q0Vec.x;
		q0Vec.y = -1.0f * q0Vec.y;
		q0Vec.z = -1.0f * q0Vec.z;
		q0W = -1.0f * q0W;
		dot = -1.0f * dot;
	}

	float theta = std::acos(dot);

	float scale0 = std::sin((1.0f - t) * theta) / std::sin(theta);
	float scale1 = std::sin(t * theta) / std::sin(theta);

	if (dot >= 1.0f - std::numeric_limits<float>::epsilon()) {
		result.x = (1.0f - t) * q0Vec.x + t * q1Vec.x;
		result.y = (1.0f - t) * q0Vec.y + t * q1Vec.y;
		result.z = (1.0f - t) * q0Vec.z + t * q1Vec.z;
		result.w = (1.0f - t) * q0W + t * q1.w;
	}
	else {
		result.x = scale0 * q0Vec.x + scale1 * q1Vec.x;
		result.y = scale0 * q0Vec.y + scale1 * q1Vec.y;
		result.z = scale0 * q0Vec.z + scale1 * q1Vec.z;
		result.w = scale0 * q0W + scale1 * q1.w;
	}

	return 	result;
}

// 演算子のオーバーロード

Matrix4x4 operator*(const Matrix4x4& a, const Matrix4x4& b)
{
	return Multiply(a, b);
}

Vector3 operator+(const Vector3& a, const Vector3& b) {
	Vector3 c = { a.x + b.x,a.y + b.y ,a.z + b.z };

	return c;
}

Vector3 operator+(const Vector3& a, const float& b) {
	Vector3 c = { a.x + b,a.y + b,a.z + b };

	return c;
}

Vector3 operator-(const Vector3& a, const Vector3& b) {
	Vector3 c = { a.x - b.x,a.y - b.y,a.z - b.z };

	return c;
}

Vector3 operator-(const Vector3& a, const float& b) {
	Vector3 c = { a.x - b,a.y - b,a.z - b };

	return c;
}

Vector3 operator*(const float& a, const Vector3& b) {
	Vector3 c = { a * b.x,a * b.y,a * b.z };

	return c;
}

Vector3 operator/(const Vector3& a, const float& b) {
	Vector3 c = { a.x / b , a.y / b, a.z / b };

	return c;
}

Vector3 operator*(const Vector3& vec, const Matrix4x4& mat) {
	Vector4 result = {
		vec.x * mat.m[0][0] + vec.y * mat.m[1][0] + vec.z * mat.m[2][0] + mat.m[3][0],
		vec.x * mat.m[0][1] + vec.y * mat.m[1][1] + vec.z * mat.m[2][1] + mat.m[3][1],
		vec.x * mat.m[0][2] + vec.y * mat.m[1][2] + vec.z * mat.m[2][2] + mat.m[3][2],
		vec.x * mat.m[0][3] + vec.y * mat.m[1][3] + vec.z * mat.m[2][3] + mat.m[3][3]
	};

	return { result.x / result.w, result.y / result.w, result.z / result.w };
}
