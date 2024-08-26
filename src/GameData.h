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
	void updateGameData(class ClientInstance* clientInstance);
	void updateGameData(unsigned char key, bool isDown) {
		this->keymap[key] = isDown;
	}
	static class ClientInstance* getClientInstance();
	static class GameMode* getGameMode();
	static class LocalPlayer* getLocalPlayer();
	static class GuiData* getGuiData();
	static bool isKeyDown(unsigned int key) {
		return g_Data.keymap[key];
	}
};

class ClientData { //クライアントのデータ
public:
	const char* getName();
	const char* getVersion();
	const char* getChatLogo();
};
extern ClientData client;