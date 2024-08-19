#pragma once
#include "../Module.h"

class TestModule : public Module {
public:
	TestModule();
	bool _bool = true;
	int _int = 3;
	float _float = 6.7f;
	Keybind _keybind = 'K';
	vec3<float> unsupported;

	void onEnableEvent()  override {
		logF("Niqqa");
	}
};