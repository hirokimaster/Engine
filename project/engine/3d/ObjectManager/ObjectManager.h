#pragma once
#include <list>
#include <memory>
#include <map>
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
	void Draw(const Camera& camera);

	/// <summary>
	/// objectの追加
	/// </summary>
	/// <param name="object"></param>
	void PushObject(std::unique_ptr<BaseObject> object) { objects_.push_back(std::move(object)); }

#pragma region getter

	BaseObject* GetObjectData(const std::string& name);

#pragma endregion

private:
    std::list<std::unique_ptr<BaseObject>> objects_; // 全てのobject
	std::map<std::string, std::queue<BaseObject*>> object_; // objectのポインタ
};
