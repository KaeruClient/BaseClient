#include "Module.h"

Module::Module(Category category, Keybind keybind, const char* tooltip) {
	this->category = category;
	this->keybind = keybind;
	this->tooltip = tooltip;
}