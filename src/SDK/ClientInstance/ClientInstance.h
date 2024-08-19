#pragma once
#include <Windows.h>
#include "GuiData.h"

class ClientInstance {
public:
	auto getLoopBackPacketSender() -> class LoopBackPacketSender* {
		return hat::member_at<class LoopBackPacketSender*>(this, Offset::ClientInstance::loopbackpacketsender);
	}
	auto getGuiData() -> class GuiData* {
		return hat::member_at<class GuiData*>(this, Offset::ClientInstance::guidata);
	}
	auto getLocalPlayer() -> class LocalPlayer* {
		return CallVFunc<Index::ClientInstance::getLocalPlayer, class LocalPlayer*>(this);
	}

	class BlockSource* getBlockSource() {
		return CallVFunc<Index::ClientInstance::getRegion, BlockSource*>(this); //ClientInstance::getRegion();
	}

	auto grabCursor() -> void {
		return CallVFunc<Index::ClientInstance::grabMouse, void>(this); //ClientInstance::grabMouse(); 1.21.2
	}

	auto releaseCursor() -> void{
		return CallVFunc<Index::ClientInstance::releaseMouse, void>(this); //ClientInstance::releaseMouse(); 1.21.2
	}
};