/**
* @file Mathfunction.h
* @brief 数学関数まとめ（グローバル関数）
* @author 仁平 琉乃
*/

#pragma once
#include "Matrix4x4.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include <cassert>
#include <numbers>
#include <vector>
#include <algorithm>

struct AABB {
	Vector3 min; // 最小点
	Vector3 max; // 最大点
};

struct Quaternion {
	float x;
	float y;
	float z;
	float w;
};

struct QuaternionTransform {
	Vector3 scale;
	Quaternion rotate;
	Vector3 translate;
};

struct UVTransform {
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};

// 積
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);
// 和
Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2);
// 差
Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2);
// 拡大縮小行列
Matrix4x4 MakeScaleMatrix(const Vector3& scale);
// 平行移動
Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

/*---------------------------------
 回転行列
------------------------------------*/

// X軸
Matrix4x4 MakeRotateXMatrix(float radian);
// Y軸
Matrix4x4 MakeRotateYMatrix(float radian);
// Z軸
Matrix4x4 MakeRotateZMatrix(float radian);

// アフィン変換
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& translate);

// quaternionを使ったアフィン変換
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Quaternion& rotate, const Vector3& translate);

// ビルボード用のワールド行列作成
Matrix4x4 MakeBiilboardWorldMatrix(const Vector3& scale, const Matrix4x4& billboard, const Vector3& translate);

// 透視投影行列
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);

//正射影行列
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);

// 逆行列
Matrix4x4 Inverse(const Matrix4x4& m);

// 単位行列
Matrix4x4 MakeIdentityMatrix();

// 逆転置行列
Matrix4x4 InverseTranspose(const Matrix4x4& m);

// 転置行列
Matrix4x4 Transpose(const Matrix4x4& m);

Matrix4x4 MakeViewportMatrix(
	float left, float top, float width, float heght, float minDepth, float maxDepth);

// 正規化
Vector3 Normalize(const Vector3& v);

// 変換
Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

// ノルム
float Length(const Vector3& v);

// ノルム(二次元)
float Length(const Vector2& v);

// ベクトル減算
Vector3 Subtract(const Vector3& v1, const Vector3& v2);

// スカラー倍
Vector3 Multiply(float scalar, const Vector3& v);

// クロス積
Vector3 Cross(const Vector3& v1, const Vector3& v2);

// 加算
Vector3 Add(const Vector3& v1, const Vector3& v2);

// 減算
Vector3 Subtract(const Vector3& v1, const Vector3& v2);

// 内積
float Dot(const Vector3& v1, const Vector3& v2);

// 線形補間
Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);

// 1軸の正規化
float NormalizeRotation(float rotation);

// 球面補間
Vector3 SLerp(const Vector3& v1, const Vector3& v2, float t);

// ベクトル変換
Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

Matrix4x4 MakeRotateMatrix(const Vector3& radian);

Matrix4x4 MakeViewMatrix(const Vector3& eye, const Vector3& target, const Vector3& up);

/*---------------------------------------
		   Quaternion
-----------------------------------------*/

// quaternionの積
Quaternion Multiply(const Quaternion& lhs, const Quaternion& rhs);
// 単位quaternion
Quaternion IdentityQuaternion();
// 共役quaternion
Quaternion Conjugate(const Quaternion& quaternion);
// quaternionのnorm
float Norm(const Quaternion& quaternion);
// 正規化したquaternion
Quaternion QNormalize(const Quaternion& quaternion);
// 逆quaternion
Quaternion QInverse(const Quaternion& quaternion);
// 任意軸回転を表すquaternionの生成
Quaternion MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle);
// ベクトルをquaternionで回転させた結果のベクトルを求める
Vector3 RotateVector(const Vector3& vector, const Quaternion& quaternion);
// quaternionから回転行列を求める
Matrix4x4 MakeRotateMatrix(const Quaternion& quaternion);
// 球面線形補間
Quaternion Slerp(const Quaternion& q0, const Quaternion& q1, float t);
// 回転を計算
Quaternion CalculateRotationQuaternion(const Vector3& from, const Vector3& to);

// catmulrom補間
Vector3 CatmullRomInterpolation(const Vector3& p_mi1,const Vector3& p_0, const Vector3& p_1, const Vector3& p_2, float t);

// catmulrom補間
Vector3 CatmullRomPosition(const std::vector<Vector3>& points, float t);

// 演算子のオーバーロード
Matrix4x4 operator*(const Matrix4x4& a, const Matrix4x4& b);
Vector3 operator+(const Vector3& a, const Vector3& b);
Vector3 operator+(const Vector3& a, const float& b);
Vector3 operator-(const Vector3& a, const Vector3& b);
Vector3 operator-(const Vector3& a, const float& b);
Vector3 operator*(const float& a, const Vector3& b);
Vector3 operator/(const Vector3& a, const float& b);
Vector3 operator*(const Vector3& vec, const Matrix4x4& mat);
Vector2 operator-(const Vector2& v1, const Vector2& v2);