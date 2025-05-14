/**
* @file BulletObjectPool.h
* @brief 弾のオブジェクトプール
* @author 仁平 琉乃
*/

#pragma once
#include "application/GameObject/Bullet/PlayerBullet/PlayerBullet.h"
#include "application/GameObject/Bullet/EnemyBullet/EnemyBullet.h"
#include "application/GameObject/Bullet/BulletTrajectory/BulletTrajectory.h"

class BulletObjectPool {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

#pragma region getter

	/// <summary>
	/// プールにある弾の取得
	/// </summary>
	/// <param name="name"></param>
	/// <returns></returns>
	IBullet* GetBullet(const std::string& name);

	/// <summary>
	/// 弾のインスタンス
	/// </summary>
	/// <returns></returns>
	const std::vector<std::unique_ptr<IBullet>>& GetBullets() const { return bullets_; }

#pragma endregion

private:

	/// <summary>
	/// 生成
	/// </summary>
	void Create(const std::string& name);

	/// <summary>
	/// キューに再追加
	/// </summary>
	void Push(const std::string& name, IBullet* bullet);

private:

	std::vector<std::unique_ptr<IBullet>> bullets_; // 全ての弾
	std::map<std::string, std::queue<IBullet*>> pool_; // 使用可能のやつを入れとく
	static const uint32_t kPoolSize = 600; // poolのサイズ
	std::unique_ptr<BulletTrajectory> trajectory_; // 弾の軌跡
	
};
