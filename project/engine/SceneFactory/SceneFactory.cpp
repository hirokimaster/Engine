/**
* @file SceneFactory.cpp
* @brief シーンの生成を行うクラス
* @author 仁平 琉乃
*/

#include "SceneFactory.h"

std::unique_ptr<IScene> SceneFactory::CreateScene(const std::string& sceneName)
{
	std::unique_ptr<IScene> newScene = nullptr;

	if (sceneName == "GAME") {
		newScene = std::make_unique<GameScene>();	
	}
	else if (sceneName == "DEMO") {
		newScene = std::make_unique<Demo>();
	}
	else if (sceneName == "TITLE") {
		newScene = std::make_unique<TitleScene>();
	}
	else if (sceneName == "CLEAR") {
		newScene = std::make_unique<ClearScene>();
	}

	return	newScene;
}