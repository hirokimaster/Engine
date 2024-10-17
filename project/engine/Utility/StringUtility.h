/**
* @file StringUtility.h
* @brief 出力にログを出す
* @author 仁平 琉乃
*/

#pragma once
#include <string>
#include <Windows.h>

/// <summary>
/// stringからwstring
/// </summary>
/// <param name="str"></param>
/// <returns></returns>
std::wstring ConvertString(const std::string& str);

/// <summary>
/// wstringからstring
/// </summary>
/// <param name="str"></param>
/// <returns></returns>
std::string  ConvertString(const std::wstring& str);

/// <summary>
/// log出す
/// </summary>
/// <param name="message"></param>
void Log(const std::string& message);

/// <summary>
/// log出す
/// </summary>
/// <param name="message"></param>
void Log(const std::wstring& message);
