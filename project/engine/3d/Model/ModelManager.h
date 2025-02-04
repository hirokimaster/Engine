/**
* @file ModelManager.h
* @brief モデルの読み込み、生成を行う。一度生成したら同じインスタンスを使いまわせる
* @author 仁平 琉乃
*/

#pragma once
#include <fstream>
#include <sstream>
#include "engine/3d/Model/Model.h"
#include "engine/3d/Model/Animation/ModelAnimation.h"
#include <numbers>
#include <map>

class ModelManager {
public:

	/// <summary>
	/// インスタンス
	/// </summary>
	/// <returns></returns>
	static ModelManager* GetInstance();

	/// <summary>
	/// objモデルのロード
	/// </summary>
	/// <param name="fileName"></param>
	/// <returns></returns>
	static void LoadObjModel(const std::string& fileName);

	/// <summary>
	/// gltfモデルのロード
	/// </summary>
	/// <param name="fileNama"></param>
	static void LoadGLTFModel(const std::string& fileName);

	/// <summary>
	/// animationを含んだモデルのロード
	/// </summary>
	/// <param name="fileName"></param>
	static void LoadAnimationModel(const std::string& fileName);

	/// <summary>
	/// ロードしたobjモデルで生成
	/// </summary>
	/// <param name="fileName"></param>
	/// <returns></returns>
	static Model* CreateObj(const std::string& fileName);

	/// <summary>
	/// ロードしたgltfでモデルを生成
	/// </summary>
	/// <param name="fileName"></param>
	/// <returns></returns>
	static Model* CreateGLTF(const std::string& fileName);

	/// <summary>
	/// ロードしたアニメーションモデルを生成
	/// </summary>
	/// <param name="fileName"></param>
	/// <returns></returns>
	static ModelAnimation* Create(const std::string& fileName);

private:
	ModelManager() = default;
	~ModelManager() = default;
	ModelManager(const ModelManager&) = delete;
	ModelManager& operator=(const ModelManager&) = delete;

private:
	std::map<std::string, std::unique_ptr<Model>>  models_;
	std::map<std::string, std::unique_ptr<ModelAnimation>> animModels_;
};
