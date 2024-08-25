#pragma once
#include <windows.h>
#include "../include/Json.hpp"

namespace JsonUtils {
	template <typename T>
	auto writeJson(nlohmann::json* json, const char* varName, T value) -> void {
		json->emplace(varName, value);
	}
	template <typename T>
	auto readJson(nlohmann::json json, const char* varName) -> T {
		return json.at(varName).get<T>();
	}
}