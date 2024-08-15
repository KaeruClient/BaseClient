#include "ModuleHandler.h"
ModuleHandler modules = ModuleHandler();

void ModuleHandler::Initialize() {
	addModule<TestModule>();
}