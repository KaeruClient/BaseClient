#include "GameData.h"
#include "include/xorstr.hpp"
#include <string>

GameData g_Data = GameData();

static std::string version = xorstr_("1.0.0");
static std::string name = xorstr_("BaseClient");

const char* ClientData::getClientName() {
	return name.c_str();
}
const char* ClientData::getClientVersion() {
	return version.c_str();
}