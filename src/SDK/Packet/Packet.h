#pragma once
#include <cstdint>
#include <string>
#include "../Signatures.h" 

class Packet {
public:
	uintptr_t** vTable;  //0x0000
private:
	char pad_0x8[0x28];
public:
	std::string getName();
	template <class T>
	bool is() {
		T packet;
		if (packet.vTable == this->vTable)
			return true;
		else
			return false;
	}
	template <typename T>
	auto to() {	
		return reinterpret_cast<T*>(this);
	}
};
