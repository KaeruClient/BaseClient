#include "TestModule.h"

TestModule::TestModule() : Module(Category::Misc,  0x0, xorstr_("Yeah, it's tooltip.")) {
	addSetting<bool>(xorstr_("BoolSetting"), &_bool);
	addSetting<int>(xorstr_("IntSetting"), &_int, 2.f, 10.f);
	addSetting<float>(xorstr_("FloatSetting"), &_float, 2.f, 10.f);
	addSetting<Keybind>(xorstr_("KeybindSetting"), &_keybind);
	//addSetting<vec3<float>>("UnsupportedSetting", &unsupported);
}