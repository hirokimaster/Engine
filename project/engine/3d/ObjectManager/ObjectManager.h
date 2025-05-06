#pragma once
#include <list>
#include <memory>
#include <map>
#include "engine/3d/Object3DPlacer/Object3DPlacer.h"
#include "application/Loader/Loader.h"

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
	/// インスタンス生成あったら値だけ取る
	/// </summary>
	/// <param name="modelName"></param>
	/// <param name="texHandle"></param>
	/// <returns></returns>
	std::shared_ptr<Object3dData> CreateInstance(const std::string& modelName, uint32_t texHandle);

	/// <summary>
	///  mapの中を空にする
	/// </summary>
	void ClearObjectMap();


private:
	// モデルとインスタンス
	std::unordered_map<std::string, std::unique_ptr<Object3DPlacer>> objectMap_;
	
};
