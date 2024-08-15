#pragma once
#include "../Hook.h"

namespace KeyboardHook {
	class Feed : public Hook {
	public:
		Feed() : Hook(xorstr_("Keyboard::feed")) {};
		static void handle(unsigned char key, bool isDown);
		bool Initialize() override {
			return CreateHook(funcPtr, Signature::keyboard_feed, handle);
		}
	};
}