#pragma once
#include <list>
#include <memory>
#include <map>
#include "engine/3d/Object3dPlacer/Object3dPlacer.h"

class ObjectManager {
public:

	/// <summary>
	/// シングルトン
	/// </summary>
	/// <returns></returns>
	static ObjectManager* GetInstance();

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
	/// インスタンス生成あったら値だけ取る
	/// </summary>
	/// <param name="modelName"></param>
	/// <param name="texHandle"></param>
	/// <returns></returns>
	std::shared_ptr<Object3dInstancing> CreateInstancingObject(const std::string& modelName, uint32_t texHandle);

	/// <summary>
	/// インスタンスが1個のみのObject
	/// </summary>
	/// <param name="modelName"></param>
	/// <param name="texHandle"></param>
	/// <returns></returns>
	std::shared_ptr<Object3dPlacer> CreateIndividualObject(const std::string& modelName, uint32_t texHandle);

	/// <summary>
	///  mapの中を空にする
	/// </summary>
	void ClearObject();


private:
	// モデルとインスタンス
	std::unordered_map<std::string, std::unique_ptr<Object3dPlacer>> instancingObjects_;
	std::vector<std::shared_ptr<Object3dPlacer>> individualObjects_;

private:
	ObjectManager() = default;
	~ObjectManager() = default;
	ObjectManager(const ObjectManager&) = delete;
	ObjectManager& operator=(const ObjectManager&) = delete;
	
};
