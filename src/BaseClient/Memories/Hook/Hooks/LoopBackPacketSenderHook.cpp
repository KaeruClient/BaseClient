#include "LoopBackPacketSenderHook.h"
#include "../../../Features/Command/CommandHandler.h"

void LoopBackPacketSenderHook::SendToServer::handle(LoopbackPacketSender* _this, Packet* packet) {
	static auto oFunc = funcPtr->GetOrigFunc<void, LoopbackPacketSender*, Packet*>();

	if (packet->is<TextPacket>())
	{
		auto p = packet->to<TextPacket>();

		char* message = p->getMessage().data();
		std::string prefix = &commands.prefix;
		if (p->getMessage().substr(0, 1) == prefix) {
			commands.execute(message);
			return;
		}
	}

	return oFunc(_this, packet);
}