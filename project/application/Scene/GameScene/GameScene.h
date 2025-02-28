/**
* @file GameScene.h
* @brief ゲームシーン
* @author 仁平 琉乃
*/

#pragma once
#include "application/Scene/IScene/IScene.h"
#include "engine/Graphics/PostProcess/PostProcess.h"
#include "application/GameObject/Player/Player.h"
#include "application/GameObject/Enemy/Enemy.h"
#include "engine/Utility/CollisionManager/CollisionManager.h"
#include "application/GameObject/Rail/Rail.h"
#include "application/GameObject/LockOn/LockOn.h"
#include "application/Loader/Loader.h"
#include "application/GameObject/FollowCamera/FollowCamera.h"
#include "application/GameObject/Skydome/Skydome.h"
#include "application/SceneTransition/SceneTransition.h"
#include "application/GameObject/Gimmick/Laser.h"
#include "application/SceneSprite/GameSprite.h"
#include "application/PostEffect/PostEffect.h"
#include "application/PostEffect/EffectState/PlayerDamageState.h"

class GameScene : public IScene {
public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize()override;

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update()override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw()override;

	/// <summary>
	/// ポストエフェクト描画
	/// </summary>
	void PostProcessDraw()override;

private:

	/// <summary>
	/// collision書くとこ
	/// </summary>
	void Collision();

	/// <summary>
	/// テクスチャ読み込む場所
	/// </summary>
	void LoadTextureFile();

	/// <summary>
	/// ゲーム開始
	/// </summary>
	void StartGame();

	/// <summary>
	/// ゲームオーバー
	/// </summary>
	void GameOver();

private:
	std::unique_ptr<Player> player_ = nullptr; // player
	std::unique_ptr<CollisionManager> collisionManager_ = nullptr; // 衝突判定まとめ
	std::unique_ptr<LockOn> lockOn_ = nullptr; // ロックオン
	std::unique_ptr<Loader> loader_ = nullptr;
	std::unique_ptr<FollowCamera> followCamera_ = nullptr;
	std::unique_ptr<GameSprite> gameSprite_ = nullptr; // ゲームシーンのスプライト

	// シーン遷移用
	bool isTransition_ = false;
	uint32_t sceneTimer_ = 100;
	bool isTransitionClear_ = false;
	SceneTransition* sceneTransition_ = nullptr;

	std::unique_ptr<PostEffect> postEffect_ = nullptr; // postEffect
	
	bool isGameStart_ = false;
	
	// カメラワーク用
	// 初期のカメラのオフセットと回転
	Vector3 offsetStart_ = { -12.0f, 0.5f, 13.0f };
	Vector3 cameraRotateStart_ = { 0, std::numbers::pi_v<float> *3.0f / 4.0f, 0 };
	// 目的のカメラのオフセットと回転
	Vector3 offsetEnd_ = { 0, 1.5f, -25.0f };
	Vector3 cameraRotateEnd_ = { 0, 0, 0 };
	float rotateParam_ = 0.0f;
	// ゲームオーバー用
	bool isGameOver_ = false;
	// 天球
	std::unique_ptr<Skydome> skydome_ = nullptr;
};
