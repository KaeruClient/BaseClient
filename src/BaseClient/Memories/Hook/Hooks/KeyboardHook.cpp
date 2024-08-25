#include "KeyboardHook.h"

void KeyboardHook::Feed::handle(unsigned char key, bool isDown) {
	static auto oFunc = funcPtr->GetOrigFunc<void, unsigned char, bool>();
	g_Data.updateGameData(key, isDown);
	bool shouldCancel = false;
	modules.flushKeyFeedEvent(key, isDown, shouldCancel);
	if (shouldCancel) isDown = false;
	return oFunc(key, isDown);
}