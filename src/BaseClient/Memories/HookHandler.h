#pragma once
#include <vector>
#include "Hook/Hook.h"
#include "Hook/Hooks/KeyboardHook.h"

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