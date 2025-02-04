#pragma once
#include "engine/2d/Sprite/Sprite.h"
#include "application/GameManager/GameManager.h"

// 前のシーン
enum class PreScene {
	Title,
	Game,
	Clear,
};

class SceneTransition {
public:

	/// <summary>
	/// シングルトン
	/// </summary>
	/// <returns></returns>
	static SceneTransition* GetInstance();

	/// <summary>
	/// 初期化(どっかで1回やればいい)
	/// </summary>
	void Initialize();

	/// <summary>
	/// フェードイン
	/// </summary>
	void FadeIn(const std::string& SceneName);

	/// <summary>
	/// フェードアウト
	/// </summary>
	void FadeOut();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

#pragma region setter

	void SetPreScene(PreScene preScene) { preScene_ = preScene; }

#pragma endregion

#pragma region getter

	PreScene GetPreScene() { return preScene_; }

#pragma endregion

private:

	SceneTransition() = default;
	~SceneTransition() = default;
	SceneTransition(const SceneTransition&) = delete;
	const SceneTransition& operator=(const SceneTransition&) = delete;

private:
	// シーン遷移時に出すsprite
	std::unique_ptr<Sprite> spriteWhite_ = nullptr;
	float alpha_ = 0.0f;
	bool fadeState_ = false; // falseの時in,trueの時out
	PreScene preScene_ = PreScene::Title; // 最初はタイトル
};
