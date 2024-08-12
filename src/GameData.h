#pragma once
#include <Windows.h>
#include <map>

extern class GameData g_Data;
class GameData { //マイクラのデータ
private:
	class ClientInstance* clientInstace = nullptr;
	class GameMode* gameMode = nullptr;
	class LocalPlayer* localPlayer = nullptr;
	class GuiData* guiData = nullptr;
	std::map<unsigned char, bool> keymap;
public:
	void updateGameData(class ClientInstance* clientInstance) {
		this->clientInstace = clientInstance;
	}
	void updateGameData(unsigned char key, bool isDown) {
		this->keymap[key] = isDown;
	}
	static class ClientInstance* getClientInstance() { return g_Data.clientInstace; }
	static class GameMode* getGameMode() { return g_Data.gameMode; }
	static class LocalPlayer* getLocalPlayer() { return g_Data.localPlayer; }
	static class GuiData* getGuiData() { return g_Data.guiData; }
	static bool isKeyDown(unsigned int key) {
		return g_Data.keymap[key];
	}
};

class ClientData { //クライアントのデータ
public:
	static const char* getClientName();
	static const char* getClientVersion();
};