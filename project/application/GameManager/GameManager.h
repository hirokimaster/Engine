/**
* @file GameManager.h
* @brief ゲームやシーンのマネージャ
* @author 仁平 琉乃
*/

#pragma once
#include <memory>
#include "engine/SceneFactory/AbstractSceneFactory.h"
#include "engine/Graphics/PostProcess/PostProcess.h"
#include "application/AdjustmentVariables/AdjustmentVariables.h"
#include "engine/Graphics/Effects/Particle/ParticleManager.h"
#include "application/SceneTransition/FadeIn/FadeIn.h"
#include "application/SceneTransition/FadeOut/FadeOut.h"
#include "engine/3d/ObjectManager/ObjectManager.h"

class GameManager {
public:

	/// <summary>
	/// シングルトンインスタンス取得
	/// </summary>
	/// <returns></returns>
	static GameManager* GetInstance();

	/// <summary>
	/// ゲームループを呼び出す
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// ポストエフェクト描画
	/// </summary>
	void PostProcessDraw();

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

#pragma region setter

	void SetSceneFactory(std::unique_ptr<AbstractSceneFactory> sceneFactory) { sceneFactory_ = std::move(sceneFactory); }

	/// <summary>
	/// postprocessのポインタを設定する
	/// </summary>
	/// <param name="postProcess"></param>
	void SetPostProcess(PostProcess* postProcess) { postProcess_ = postProcess; }

	void SetSceneTransition(ISceneTransition* transition) { transition_ = transition; }

#pragma endregion

	/// <summary>
	/// シーン変更
	/// </summary>
	/// <param name="sceneName"></param>
	void ChangeScene(const std::string& sceneName);

#pragma region getter

	/// <summary>
	/// postprocessのポインタを取得する
	/// </summary>
	/// <returns></returns>
	PostProcess* GetPostProcess() { return postProcess_; }

#pragma endregion

private:
	GameManager() = default;
	~GameManager() = default;
	GameManager(const GameManager&) = delete;
	GameManager& operator=(const GameManager&) = delete;

private:

	std::unique_ptr<IScene> scene_; // 今のシーン
	std::unique_ptr<IScene> nextScene_; // 次のシーン
	std::unique_ptr<AbstractSceneFactory> sceneFactory_;
	PostProcess* postProcess_ = nullptr;
    ISceneTransition* transition_; // シーン遷移
};
