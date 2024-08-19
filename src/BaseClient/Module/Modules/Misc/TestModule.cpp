#include "TestModule.h"

TestModule::TestModule() : Module(Category::Misc,  0x0, xorstr_("Yeah, it's tooltip.")) {
	registerSetting<bool>(xorstr_("BoolSetting"), &_bool);
	registerSetting<int>(xorstr_("IntSetting"), &_int, 2.f, 10.f);
	registerSetting<float>(xorstr_("FloatSetting"), &_float, 2.f, 10.f);
	registerSetting<Keybind>(xorstr_("KeybindSetting"), &_keybind);
	//addSetting<vec3<float>>("UnsupportedSetting", &unsupported);
}