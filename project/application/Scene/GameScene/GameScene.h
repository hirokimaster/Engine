#pragma once
#include "application/Scene/IScene/IScene.h"
#include "engine/Sprite/Sprite.h"
#include "engine/Model/Model.h"
#include "engine/Input/Input.h"
#include "engine/PostProcess/PostProcess.h"
#include "engine/Model/Animation/ModelAnimation.h"
#include "engine/Object3DPlacer/Object3DPlacer.h"
#include "application/GameManager/GameManager.h"
#include "application/GameObject/Player/PlayerManager.h"
#include "application/GameObject/ModelResources/ModelResources.h"
#include "application/GameObject/Enemy/EnemyManager.h"
#include "engine/Utility/CollisionManager/CollisionManager.h"
#include "application/GameObject/RailCamera/RailCamera.h"

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

private:
	Camera camera_{};
	std::unique_ptr<PlayerManager> playerManager_ = nullptr; // playerまとめたとこ
	std::unique_ptr<EnemyManager> enemyManager_ = nullptr; // enemyまとめたとこ
	std::unique_ptr<CollisionManager> collisionManager_ = nullptr; // 衝突判定まとめ
	std::unique_ptr<RailCamera> railCamera_ = nullptr; // レールカメラ
};
