#pragma once
#include "AbstractSceneFactory.h"
#include "application/Scene/GameScene/GameScene.h"

class SceneFactory : public AbstractSceneFactory {
public:
	/// <summary>
	/// シーン生成
	/// </summary>
	/// <param name="sceneName"></param>
	/// <returns></returns>
	std::unique_ptr<IScene> CreateScene(const std::string& sceneName)override;

};
