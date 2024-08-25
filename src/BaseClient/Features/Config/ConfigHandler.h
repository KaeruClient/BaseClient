#pragma once
#include <Windows.h>
#include <fstream>
#include <filesystem>
#include "../../../include/Json.hpp"

class ConfigHandler {
private:
	static nlohmann::json jsonConfig;
	static inline constexpr const char* defaultConfig = "Config";
public:
	static inline std::string fileName = defaultConfig;
	static bool Load(std::string config, bool IfFalidCreateConfig);
	static bool Save(std::string config);
	static bool Create(std::string config);
	static void Initialize();
};
