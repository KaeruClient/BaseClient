#include "ClientInstanceHook.h"

void ClientInstanceHook::Update::handle(ClientInstance* _this, bool a2) {
	static auto oFunc = funcPtr->GetOrigFunc<void, ClientInstance*, bool>();
	static auto testModule = modules.get<TestModule>();
	static bool yeah = false;
	if (!yeah) {
		logF("ClientInstance : %llX", _this);
		yeah = true;
	}
	g_Data.updateGameData(_this);
	return oFunc(_this, a2);
}