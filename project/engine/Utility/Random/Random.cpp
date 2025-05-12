/**
* @file Random.cpp
* @brief ランダムエンジンの生成
* @author 仁平 琉乃
*/

#include "Random.h"

std::mt19937& Random::Engine()
{
	static std::mt19937 engine{ std::random_device{}() };
	return engine;
}

int32_t Random::Range(int32_t min, int32_t max)
{
	std::uniform_int_distribution<int32_t> dist(min, max);
	return dist(Engine());
}

float Random::Range(float min, float max)
{
	std::uniform_real_distribution<float> dist(min, max);
	return dist(Engine());
}


