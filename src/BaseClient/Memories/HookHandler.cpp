#include "HookHandler.h"

void HookHandler::Initialize() {
	{
		hookList.push_back(new ClientInstanceHook::Update());
		hookList.push_back(new KeyboardHook::Feed());
	}

	for (std::size_t i = 0; i < hookList.size(); ++i) {
		if (hookList[i]->Initialize()) logF("Loaded hook | %s", hookList[i]->name.c_str());
		else logF("Failed to hook | %s", hookList[i]->name.c_str());
	}
	MH_EnableHook(MH_ALL_HOOKS);
	DirectXHook::init();
}