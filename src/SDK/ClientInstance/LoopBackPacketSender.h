#pragma once
#include "../Signatures.h" 

class Packet;
class LoopbackPacketSender
{
private:
	virtual void destructor();
public:
	virtual void send(Packet*);
	virtual void sendToServer(Packet*);
};