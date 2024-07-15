#pragma once
#include <string>
#include <cassert>
#include <iostream>
#include <fstream>
#include "externals/Json/json.hpp"

class Loader {
public:

	void Load(const std::string& fileName);

};