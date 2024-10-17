/**
* @file ShaderCompile.cpp
* @brief シェーダーを読み込むための関数
* @author 仁平 琉乃
*/

#pragma once
#include "engine/Base/DX/DirectXCommon.h"
#include <dxcapi.h>
#pragma comment(lib, "dxcompiler.lib")

class ShaderCompile {
public: // メンバ関数

	/// <summary>
	/// シェーダーをコンパイルする
	/// </summary>
	/// <param name="filePath"></param>
	/// <param name="profile"></param>
	/// <param name="dxcUtils"></param>
	/// <param name="dxcCompiler"></param>
	/// <param name="includeHandler"></param>
	/// <returns></returns>
	IDxcBlob* CompileShader(
		// CompilerするShaderファイルへのパス
		const std::wstring& filePath,
		// Compilerに使用するProfile
		const wchar_t* profile,
		// 初期化で生成したものを3つ
		IDxcUtils* dxcUtils,
		IDxcCompiler3* dxcCompiler,
		IDxcIncludeHandler* includeHandler);
};

