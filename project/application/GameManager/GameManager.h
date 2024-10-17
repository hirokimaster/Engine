#pragma once
#include <memory>
#include "engine/SceneFactory/AbstractSceneFactory.h"
#include "engine/PostProcess/PostProcess.h"

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
	/// 初期化
	/// </summary>
	void Initialize(); // 初期化

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

	std::unique_ptr<IScene> scene_ = nullptr; // 今のシーン
	std::unique_ptr<IScene> nextScene_ = nullptr; // 次のシーン
	std::unique_ptr<AbstractSceneFactory> sceneFactory_ = nullptr;
	PostProcess* postProcess_ = nullptr;

};
