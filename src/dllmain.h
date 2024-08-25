#pragma once
#include "dllmain.h"

class dllmain {
public:
	static bool isRunning;
	static std::unique_ptr<discord::Core> core;
};