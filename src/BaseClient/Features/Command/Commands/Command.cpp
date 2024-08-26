#include "Command.h"
#include <cstdarg>
#include <string>
#include <vector>
#include <algorithm>

Command::Command(const char* command, const char* description, const char* usage) {
	this->_command = command;
	this->_description = description;
	this->_usage = usage;
	registerAlias(command);
}

Command::~Command() {
}
void Command::clientMessageF(const char* fmt, ...) {
	va_list arg;
	va_start(arg, fmt);
	//g_Data.getGuiData()->displayClientMessageVA(fmt, arg);
	va_end(arg);
}
void Command::registerAlias(const char* str) {
	std::string ss = str;
	std::transform(ss.begin(), ss.end(), ss.begin(), ::tolower);
	aliasList.push_back(ss);
}