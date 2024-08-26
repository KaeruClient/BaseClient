#pragma once

#include "Command.h"
#include "../../Config/ConfigHandler.h"

class ConfigCommand : public Command {
public:
	ConfigCommand();
	~ConfigCommand();

	// Inherited via IMCCommand
	virtual bool execute(std::vector<std::string>* args) override;
};