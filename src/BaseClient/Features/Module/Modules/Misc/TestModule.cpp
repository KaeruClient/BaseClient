#include "TestModule.h"

TestModule::TestModule() : Module("TestModule", Category::Misc, 0x0, "Yeah, it's tooltip.") {
	registerSetting<bool>("BoolSetting", "The boolean setting.", & _bool);
	registerSetting<int>("IntSetting", "The intger setting.", &_int, 2.f, 10.f);
	registerSetting<float>("FloatSetting", "The float setting.", &_float, 2.f, 10.f);
	registerSetting<Keybind>("KeybindSetting", "The keybind setting.", &_keybind);

}