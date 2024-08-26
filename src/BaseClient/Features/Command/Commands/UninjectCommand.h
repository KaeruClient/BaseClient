#pragma once

#include "Command.h"
#include "../../../../dllmain.h"

class UninjectCommand : public Command {
public:
	UninjectCommand();
	~UninjectCommand();

	// Inherited via IMCCommand
	virtual bool execute(std::vector<std::string>* args) override;
};