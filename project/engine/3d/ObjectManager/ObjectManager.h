#pragma once
#include <list>
#include <memory>
#include "engine/3d/Object3DPlacer/BaseObject.h"

class ObjectManager {
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
	void Draw();

	/// <summary>
	/// オブジェクトの追加
	/// </summary>
	/// <param name="objects"></param>
	void PushObject(std::unique_ptr<BaseObject> objects) { objects_.push_back(std::move(objects)); }

	/// <summary>
	/// objectの情報
	/// </summary>
	void LoadObjectData();

private:

	std::list<std::unique_ptr<BaseObject>> objects_;

};
