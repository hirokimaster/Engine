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

	return	newScene;
}