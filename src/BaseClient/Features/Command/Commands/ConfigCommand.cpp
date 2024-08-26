#include "ConfigCommand.h"

ConfigCommand::ConfigCommand() : Command("config", "Load/save configs", "<load/new/save> <name>") {
	registerAlias("c");
}

ConfigCommand::~ConfigCommand() {
}

bool ConfigCommand::execute(std::vector<std::string>* args) {
	if (args->at(1) == "load") {
		std::string name = args->at(2);
		ConfigHandler::Load(name, false);
		return true;
	}
	else if (args->at(1) == "new") {
		std::string name = args->at(2);
		ConfigHandler::Load(name, true);
		return true;
	}
	else if (args->at(1) == "save") {
		ConfigHandler::Save();
		clientMessageF("%s %sSuccessfully saved config %s%s%s!", client.getChatLogo(), GREEN, GRAY, ConfigHandler::fileName.c_str(), GREEN);
		return true;
	}
	return false;
}