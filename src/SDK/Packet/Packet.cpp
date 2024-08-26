#include "Packet.h"
std::string Packet::getName() {
	std::string str = "";
	std::string name = *CallVFunc<2, std::string*>(this, &str); 
	return name;
}