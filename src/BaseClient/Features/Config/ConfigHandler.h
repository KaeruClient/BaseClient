#pragma once
#include <Windows.h>
#include <fstream>
#include <filesystem>
#include "../../../include/Json.hpp"

class ConfigHandler {
private:
	static nlohmann::json jsonConfig;
	static inline constexpr const char* defaultConfig = "Config";
	static inline std::string fileName = defaultConfig;
public:
	static bool Load(std::string config, bool IfFalidCreateConfig);
	static bool Save();
	static void Initialize();
};
