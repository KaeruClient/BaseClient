#pragma once
#include <vector>
#include <algorithm>
#include <string>
#include "Commands/Command.h"
#include "../../../Utils/Utils.h"

#include "Commands/BindCommand.h"
#include "Commands/ConfigCommand.h"
#include "Commands/UninjectCommand.h"

extern class CommandHandler commands;
class CommandHandler {
private:
	std::vector<Command*> commandList;

public:
	char prefix = '.';

	template <typename T>
	static void add() {
		static_assert(std::is_base_of<Command, T>::value, "It isn't Command!");
		commands.commandList.push_back(new T);
		std::string name = typeid(T).name();
		logF("Module added | %s", name.substr(6, name.length()));
	};
	static void Initialize();
	static void Restore();
	std::vector<Command*>* getList();

	void execute(char* message);
};
