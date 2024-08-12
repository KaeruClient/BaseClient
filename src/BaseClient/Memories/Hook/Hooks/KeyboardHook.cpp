#include "KeyboardHook.h"

void KeyboardHook::Feed::handle(unsigned char key, bool isDown) {
	static auto oFunc = funcPtr->GetFastcall<void, unsigned char, bool>();
	g_Data.updateGameData(key, isDown);
	return oFunc(key, isDown);
}