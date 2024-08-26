#pragma once
#include "../Packet.h"
class TextPacket : public Packet {
public:
	TextPacket() { vTable = Signature::VTable::Packets::textPacket; }
	auto getMessage() -> std::string { return hat::member_at<std::string>(this, Offset::Packets::TextPacket::message); };
};