#pragma once
#include "../Signatures.h" 

class Packet;
class LoopbackPacketSender
{
public:
	void sendToServer(Packet* packet) { 
		return CallVFunc<Index::LoopBackPacketSender::sendToServer, void, Packet*>(this, packet);
	};
};