#pragma once
#include "../Hook.h"

namespace LoopBackPacketSenderHook {
	class SendToServer : public Hook {
	private:
		static inline std::unique_ptr<HookClass> funcPtr;
	public:
		SendToServer() : Hook(xorstr_("LoopBackPacketSender::sendToServer")) {};
		static void handle(LoopbackPacketSender* _this, Packet* packet);
		bool Initialize() override {
			return CreateHook(funcPtr, (uintptr_t)Signature::VTable::LoopBackPacketSender[Index::LoopBackPacketSender::sendToServer], handle);
		}
	};
}