#include "GameData.h"
#include "include/xorstr.hpp"
#include <string>

#include "SDK/SDK.h"
#include "Utils/Utils.h"

GameData g_Data = GameData();
ClientData client = ClientData();

static std::string version = xorstr_("1.0.0");
static std::string name = xorstr_("BaseClient");

const char* ClientData::getName() {
	return name.c_str();
}
const char* ClientData::getVersion() {
	return version.c_str();
}
const char* ClientData::getChatLogo() {
	std::string logo = 
		GRAY + 
		std::string("<") + 
		AMETHYST + getName() + 
		GRAY + 
		std::string(">");
	return logo.c_str();
}


void GameData::updateGameData(class ClientInstance* ci) {
	this->clientInstace = ci;
	this->guiData = ci->getGuiData();
	this->localPlayer = ci->getLocalPlayer();
}


class ClientInstance* GameData::getClientInstance() { return g_Data.clientInstace; }
class GameMode* GameData::getGameMode() { return g_Data.gameMode; }
class LocalPlayer* GameData::getLocalPlayer() { return g_Data.localPlayer; }
class GuiData* GameData::getGuiData() { return g_Data.guiData; }