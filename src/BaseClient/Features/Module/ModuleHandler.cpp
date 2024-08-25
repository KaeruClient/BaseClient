#include "ModuleHandler.h"
ModuleHandler modules = ModuleHandler();

void ModuleHandler::Initialize() {
	add<TestModule>();
	add<ArrayList>();
	logF("Initialized ModuleHandler!");

	std::sort(modules.moduleList.begin(), modules.moduleList.end(), [](auto lhs, auto rhs) {
		auto current = lhs;
		auto other = rhs;
		return std::string{ *current->getName() } < std::string{ *other->getName() };
	});
	modules.initialized = true;
	modules.get<ArrayList>()->setEnabled(true);
}