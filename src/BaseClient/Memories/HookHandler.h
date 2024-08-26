#pragma once
#include <vector>
#include "Hook/Hook.h"
#include "Hook/Hooks/KeyboardHook.h"
#include "Hook/Hooks/ClientInstanceHook.h"
#include "Hook/Hooks/LoopBackPacketSenderHook.h"
#include "Hook/Hooks/DirectXHook.h"
class HookHandler {
private:
	static inline std::vector<Hook*> hookList;
public:
	static void Initialize();
	static void Restore() { 
		logF("Restore hooks...");
		hookList.clear(); 
	}
};