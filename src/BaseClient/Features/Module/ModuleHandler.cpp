#include "ModuleHandler.h"
ModuleHandler modules = ModuleHandler();

void ModuleHandler::Initialize() {
	add<TestModule>();
	logF("Initialized ModuleHandler!");
	modules.initialized = true;
}