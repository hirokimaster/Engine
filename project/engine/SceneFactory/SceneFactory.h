/**
* @file SceneFactory.h
* @brief シーンの生成を行うクラス
* @author 仁平 琉乃
*/

#pragma once
#include "AbstractSceneFactory.h"
#include "application/Scene/GameScene/GameScene.h"
#include "application/Scene/Demo/Demo.h"
#include "application/Scene/TitleScene/TitleScene.h"
#include "application/Scene/ClearScene/ClearScene.h"

class SceneFactory : public AbstractSceneFactory {
public:
	/// <summary>
	/// シーン生成
	/// </summary>
	/// <param name="sceneName"></param>
	/// <returns></returns>
	std::unique_ptr<IScene> CreateScene(const std::string& sceneName)override;

};
