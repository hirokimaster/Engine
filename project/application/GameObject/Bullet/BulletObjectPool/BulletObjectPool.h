#pragma once
#include "application/GameObject/Bullet/PlayerBullet/PlayerBullet.h"
#include "application/GameObject/Bullet/EnemyBullet/EnemyBullet.h"

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

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	void Draw(const Camera& camera);

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
	static const uint32_t kPoolSize = 400; // poolのサイズ
};
