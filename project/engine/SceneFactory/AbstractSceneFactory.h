/**
* @file AbstractSceneFactory.h
* @brief SceneFactoryの基底クラス
* @author 仁平 琉乃
*/

#pragma once
#include "application/Scene/IScene/IScene.h"
#include <string>

class AbstractSceneFactory {
public:
	/// <summary>
	/// 仮想デストラクタ
	/// </summary>
	virtual ~AbstractSceneFactory() = default;
	
	/// <summary>
	/// シーン生成
	/// </summary>
	/// <param name="sceneName"></param>
	/// <returns></returns>
	virtual std::unique_ptr<IScene> CreateScene(const std::string& sceneName) = 0;
};