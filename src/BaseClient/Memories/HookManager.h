#pragma once
#include <vector>
#include "Hook/Hook.h"
#include "Hook/Hooks/KeyboardHook.h"

class HookManager {
private:
	static inline std::vector<Hook*> hookList;
public:
	static void Initialize();
	static void Restore() { hookList.clear(); }
};