#include "Module.h"

Module::Module(std::string moduleName, Category category, Keybind keybind, const char* tooltip) {
	registerSetting<bool>("enabled", "", &enabled);
	registerSetting<Keybind>("Keybind", "Set a key to toggle the module.", &keybind);
	this->moduleName = moduleName;
	this->category = category;
	this->keybind = keybind;
	this->tooltip = tooltip;
}