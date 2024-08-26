#pragma once

#include <map>

#include "Command.h"

class BindCommand : public Command {
public:
	BindCommand();
	~BindCommand();

	// Inherited via IMCCommand
	virtual bool execute(std::vector<std::string>* args) override;
};