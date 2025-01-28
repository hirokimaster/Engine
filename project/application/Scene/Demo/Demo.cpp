/**
* @file Demo.cpp
* @brief デモシーン
* @author 仁平 琉乃
*/

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
	camera_.Initialize();

}

void Demo::Update()
{

	camera_.UpdateMatrix();		
}

void Demo::Draw()
{
}

void Demo::PostProcessDraw()
{
}