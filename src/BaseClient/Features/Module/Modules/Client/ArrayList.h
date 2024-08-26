#pragma once
#include "../Module.h"
#include "../../ModuleHandler.h"
class ArrayList : public Module {
public:
	ArrayList();
	void onPresentEvent(ImDrawList* d) override;
};