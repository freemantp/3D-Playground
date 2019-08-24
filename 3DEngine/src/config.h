#pragma once

#include <string>
#include <filesystem>

struct Config {

public:
	
	static const std::filesystem::path DATA_BASE_PATH;
	static const std::filesystem::path TEXTURE_BASE_PATH;
	static const std::filesystem::path MODELS_BASE_PATH;	
	static const std::filesystem::path SHADER_BASE_PATH;
	static const std::filesystem::path SCENE_BASE_PATH;
};