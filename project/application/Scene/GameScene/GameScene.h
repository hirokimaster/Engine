/**
* @file GameScene.h
* @brief ゲームシーン
* @author 仁平 琉乃
*/

#pragma once
#include "application/Scene/IScene/IScene.h"
#include "engine/Utility/CollisionManager/CollisionManager.h"
#include "application/Loader/Loader.h"
#include "application/GameObject/Skydome/Skydome.h"
#include "application/SceneTransition/FadeOut/FadeOut.h"
#include "application/SceneTransition/FadeIn/FadeIn.h"
#include "application/SceneSprite/GameSprite.h"
#include "application/PostEffect/EffectState/PlayerDamageState.h"
#include "application/GameState/GameStartState.h"
#include "application/PostEffect/PostEffect.h"
#include <application/GameObject/EnemyManager/EnemyManager.h>
#include "application/GameObject/Bullet/BulletObjectPool/BulletObjectPool.h"

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

private:
	std::unique_ptr<Player> player_; // player
	std::unique_ptr<CollisionManager> collisionManager_; // 衝突判定まとめ
	std::unique_ptr<LockOn> lockOn_; // ロックオン
	std::unique_ptr<Loader> loader_;
	CameraManager* cameraManager_ = nullptr; // カメラマネージャ
	std::unique_ptr<GameSprite> gameSprite_; // ゲームシーンのスプライト
	std::unique_ptr<ISceneTransition> transition_; // 遷移
	bool isTransitionClear_ = false; // クリアに行くかどうか
	bool isGameStart_ = false; // ゲームが開始しているか
	std::unique_ptr<PostEffect> postEffect_; // postEffect
	std::unique_ptr<Skydome> skydome_; // 天球
	std::unique_ptr<IGameState> gameState_; // ゲームのスタート演出とか
	std::unique_ptr<BulletObjectPool> bulletObjectPool_; // 弾のプール
	std::unique_ptr<EnemyManager> enemyManager_; // enemy
};
