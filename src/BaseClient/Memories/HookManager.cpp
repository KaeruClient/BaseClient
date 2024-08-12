#include "HookManager.h"

void HookManager::Initialize() {
	{
		hookList.push_back(new KeyboardHook::Feed());
	}

	for (std::size_t i = 0; i < hookList.size(); ++i) {
		if (hookList[i]->Initialize()) Logger::writelog("Loaded hook | %s", hookList[i]->name);
		else Logger::writelog("Failed to hook | %s", hookList[i]->name);
	}
	MH_EnableHook(MH_ALL_HOOKS);
}