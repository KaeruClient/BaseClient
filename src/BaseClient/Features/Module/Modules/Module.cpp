#include "Module.h"

Module::Module(std::string moduleName, Category category, Keybind keybind, const char* tooltip) {
	this->moduleName = moduleName;
	this->category = category;
	this->keybind = keybind;
	this->tooltip = tooltip;
	registerSetting<bool>("enabled", "", &this->enabled);
	registerSetting<Keybind>("Keybind", "Set a key to toggle the module.", &this->keybind);
}
