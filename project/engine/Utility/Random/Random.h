/**
* @file Random.h
* @brief ランダムエンジンの生成
* @author 仁平 琉乃
*/

#pragma once
#include <random>

class Random {
public:
	/// <summary>
	/// エンジンの取得
	/// </summary>
	/// <returns></returns>
	static std::mt19937& Engine();

	/// <summary>
	/// 整数の範囲
	/// </summary>
	/// <param name="min"></param>
	/// <param name="max"></param>
	/// <returns></returns>
	static int32_t Range(int32_t min, int32_t max);

	/// <summary>
	/// 実数の範囲
	/// </summary>
	/// <param name="min"></param>
	/// <param name="max"></param>
	/// <returns></returns>
	static float Range(float min, float max);

private:
	Random() = default;
	~Random() = default;
	Random(const Random&) = delete;
	const Random& operator=(const Random&) = delete;
};
