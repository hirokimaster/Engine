#pragma once
#include "application/Scene/IScene/IScene.h"
#include "engine/Sprite/Sprite.h"
#include "engine/Model/Model.h"
#include "engine/Input/Input.h"
#include "engine/PostProcess/PostProcess.h"
#include "engine/Model/Animation/ModelAnimation.h"
#include "engine/Object3DPlacer/Object3DPlacer.h"
#include "application/GameManager/GameManager.h"
#include "application/GameObject/Player/Player.h"
#include "application/GameObject/Enemy/Enemy.h"
#include "engine/Utility/CollisionManager/CollisionManager.h"
#include "application/GameObject/Rail/Rail.h"
#include "engine/Model/SkyBox/SkyBox.h"
#include "application/GameObject/LockOn/LockOn.h"
#include "application/Loader/Loader.h"
#include "application/GameObject/FollowCamera/FollowCamera.h"
#include "application/GameObject/Enemy/BossEnemy/BossEnemy.h"

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
	/// シーン遷移
	/// </summary>
	void SceneTransition();

private:
	Camera camera_{};
	// player
	std::unique_ptr<Player> player_ = nullptr;
	uint32_t texHandlePlayer_ = 0;

	std::unique_ptr<CollisionManager> collisionManager_ = nullptr; // 衝突判定まとめ
	std::unique_ptr<SkyBox> skyBox_ = nullptr; // スカイボックス(背景)
	std::unique_ptr<LockOn> lockOn_ = nullptr; // ロックオン
	WorldTransform worldTransformSkyBox_{};
	uint32_t texHandleSkyBox_ = 0; // skyBoxのtexture
	std::unique_ptr<Loader> loader_ = nullptr;
	LevelData* levelData_ = nullptr;
	std::unique_ptr<FollowCamera> followCamera_ = nullptr;
	// 仮のUI
	std::unique_ptr<Sprite> spriteLockOn_ = nullptr;
	std::unique_ptr<Sprite> spriteUnLock_ = nullptr;
	std::unique_ptr<Sprite> spriteAttack_ = nullptr;
	uint32_t texHandleLockOn_ = 0;
	uint32_t texHandleUnLock_ = 0;
	uint32_t texHandleAttack_ = 0;

	// bossEnemy
	//std::unique_ptr<BossEnemy> bossEnemy_ = nullptr;

	// シーン遷移用
	bool isTransition_ = false;
	uint32_t sceneTimer_ = 60;
	bool isTransitionClear_ = false;

	// postEffect用
	uint32_t texHandleMask_ = 0;
	DissolveParam param_{};
	std::unique_ptr<Sprite> spriteWhite_;
	uint32_t texHandleWhite_ = 0;
	std::unique_ptr<PostProcess> postProcess_;
};