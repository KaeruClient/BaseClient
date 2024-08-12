#pragma once
#include "../Hook.h"

namespace KeyboardHook {
	class Feed : public Hook {
	private:
		static inline std::unique_ptr<FuncHook> funcPtr;
	public:
		Feed() : Hook(xorstr_("Keyboard::feed")) {};
		static void handle(unsigned char key, bool isDown);
		bool Initialize() override {
			return CreateHook(funcPtr, (void*)Signature::keyboard_feed, handle);
		}
	};
}