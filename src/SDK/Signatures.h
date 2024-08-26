#pragma once
#include <Windows.h>
#include "../Utils/MemUtils.h"
namespace Signature {
	namespace Keyboard {
		static const uintptr_t feed = SigScan("48 83 ec ? 0f b6 c1 4c 8d 05"); //Ç∆ÇËï˚ : https://www.youtube.com/watch?v=jIZnWKZwjVc 
	}
	namespace ClientInstance {
		static const uintptr_t update = SigScan("48 89 5C 24 10 48 89 74 24 18 55 57 41 54 41 56 41 57 48 8D AC 24 F0 FA"); //stringåüçı textures/ui/pointer
	}
	namespace VTable {
		namespace Packets {
			static uintptr_t** textPacket = ScanVTable("48 8D 05 ?? ?? ?? ?? 48 8B F9 48 89 01 48 8B 99 ?? ?? ?? ?? 48 8B A9", 3);
		}
		static uintptr_t** LoopBackPacketSender = ScanVTable("48 8d 05 ? ? ? ? 48 8b 5c 24 ? 48 89 06 33 c0 48 89 7e ? c6 46 ? ? 48 89 46 ? 48 89 46 ? 48 89 46 ? 48 89 46 ? 48 89 46 ? 48 89 46 ? 48 89 46 ? 48 8b c6 48 8b 74 24 ? 48 83 c4 ? 5f c3 48 89 5c 24", 3);
	}
}
namespace Offset {
	namespace ClientInstance {
		static constexpr unsigned int guidata = 0x558; //windowsize 
		static constexpr unsigned int loopbackpacketsender = 0xF0; //ClientInstance::ClientInstance
	}
	namespace Packets {
		namespace TextPacket {
			static constexpr unsigned int message = 0x58; //TextPacket::TextPacket
		}
	}
}
namespace Index {
	namespace ClientInstance {
		static constexpr int getRegion = 28;
		static constexpr int getLocalPlayer = 29;
		static constexpr int grabMouse = 332;
		static constexpr int releaseMouse = 333;
	}
	namespace LoopBackPacketSender {
		static constexpr int sendToServer = 2;
	}
}