/**
* @file Model.h
* @brief モデルの読み込み、モデルの形状についてのクラス
* @author 仁平 琉乃
*/


#pragma once
#include "engine/Graphics/Pipeline/GraphicsPipeline.h"
#include <math.h>
#include <fstream>
#include <sstream>
#include "engine/Utility/ImGuiManager/ImGuiManager.h"
#include "engine/Graphics/Camera/Camera.h"
#include "externals/assimp/include/assimp/Importer.hpp"
#include "externals/assimp/include/assimp/scene.h"
#include "externals/assimp/include/assimp/postprocess.h"
#include <numbers>
#include <map>
#include "engine/Math/Mathfunction.h"

struct Node {
	QuaternionTransform transform;
	Matrix4x4 localMatrix;
	std::string name;
	std::vector<Node> children;
};

struct MaterialData {
	std::string textureFilePath;
};

struct VertexWeightData {
	float weight;
	uint32_t vertexIndex;
};

struct JointWeightData {
	Matrix4x4 inverseBindPoseMatrix;
	std::vector<VertexWeightData> vertexWeights;
};

struct ModelData {
	std::map<std::string, JointWeightData> skinClusterData;
	std::vector<VertexData> vertices;
	std::vector<uint32_t> indices;
	MaterialData material;
	Node rootNode;
};

class Model {
public:

	~Model();

	/// <summary>
	/// obj初期化
	/// </summary>
	/// <param name="filename"></param>
	void InitializeObj(const std::string& filename);

	/// <summary>
	/// gltf初期化
	/// </summary>
	/// <param name="filename"></param>
	void InitializeGLTF(const std::string& filename);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// instancing用の描画
	/// </summary>
	/// <param name="numInstance"></param>
	void Draw(uint32_t numInstance);

#pragma region getter

	ModelData GetModelData() { return modelData_; }

#pragma endregion

	/// <summary>
	/// Objファイルを読む
	/// </summary>
	/// <param name="directoryPath"></param>
	/// <param name="filename"></param>
	/// <returns></returns>
	ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename);

	/// <summary>
	/// GLTFファイルを読む
	/// </summary>
	/// <param name="directoryPath"></param>
	/// <param name="filename"></param>
	/// <returns></returns>
	ModelData LoadGLTFFile(const std::string& directoryPath, const std::string& filename);

	/// <summary>
	/// mtlファイルを読む
	/// </summary>
	/// <param name="directoryPath"></param>
	/// <param name="filename"></param>
	/// <returns></returns>
	MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);

	/// <summary>
	/// ノードを読む
	/// </summary>
	/// <param name="node"></param>
	/// <returns></returns>
	Node ReadNode(aiNode* node);

private: // メンバ変数

	ModelData modelData_;
	Resource resource_ = {};
	D3D12_VERTEX_BUFFER_VIEW VBV_{};
	D3D12_INDEX_BUFFER_VIEW IBV_{};
};


