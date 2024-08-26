#include "CommandHandler.h"

void CommandHandler::Initialize() {
	logF("Initializing commands");

	commands.add<UninjectCommand>();
	commands.add<BindCommand>();
	commands.add<ConfigCommand>();

}

void CommandHandler::Restore() {
	commands.commandList.clear();
}

std::vector<Command*>* CommandHandler::getList() {
	return &commandList;
}

void CommandHandler::execute(char* message) {
	if (message != nullptr) {
		std::vector<std::string> args;
		std::string msgStr = message + 1;
		size_t pos = msgStr.find(" "), initialPos = 0;
		while (pos != std::string::npos) {
			args.push_back(msgStr.substr(initialPos, pos - initialPos));
			initialPos = pos + 1;

			pos = msgStr.find(" ", initialPos);
		}
		args.push_back(msgStr.substr(initialPos, min(pos, msgStr.size()) - initialPos + 1));

		std::string cmd = args[0];
		std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);

		for (auto it = this->commandList.begin(); it != this->commandList.end(); ++it) {
			Command* c = *it;
			auto* aliases = c->getAliasList();
			for (auto it = aliases->begin(); it != aliases->end(); ++it) {
				if (*it == cmd) {
					try {
						if (!c->execute(&args)) {
							//g_Data.getClientInstance()->getGuiData()->displayClientMessageF("%s%sUsage: %s%c%s %s", RED, BOLD, RESET, cmdMgr->prefix, c->getCommand(), c->getUsage(cmd.c_str()));
						}
					}
					catch (...) {
						//g_Data.getClientInstance()->getGuiData()->displayClientMessageF("%s%sUsage: %s%c%s %s", RED, BOLD, RESET, cmdMgr->prefix, c->getCommand(), c->getUsage(cmd.c_str()));
					}
					return;
				}
			}
		}

		//g_Data.getClientInstance()->getGuiData()->displayClientMessageF("[%sHorion%s] %sCommand '%s' could not be found!", GOLD, WHITE, RED, cmd.c_str());
	}
}

CommandHandler commands = CommandHandler();