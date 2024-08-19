#include "GameData.h"
#include "include/xorstr.hpp"
#include <string>

#include "SDK/SDK.h"

GameData g_Data = GameData();

static std::string version = xorstr_("1.0.0");
static std::string name = xorstr_("BaseClient");

const char* ClientData::getClientName() {
	return name.c_str();
}
const char* ClientData::getClientVersion() {
	return version.c_str();
}

void GameData::updateGameData(class ClientInstance* ci) {
	this->clientInstace = ci;
	this->guiData = ci->getGuiData();
	this->localPlayer = ci->getLocalPlayer();
}