#include "UninjectCommand.h"

UninjectCommand::UninjectCommand() : Command("Uninject", "Removes the client from the game.", "No handled error") {
	registerAlias("eject");
}

UninjectCommand::~UninjectCommand() {
}

bool UninjectCommand::execute(std::vector<std::string>* args) {
	dllmain::isRunning = false;
	return false;
}