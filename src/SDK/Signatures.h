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
}
namespace Offset {
	namespace ClientInstance {
		static constexpr unsigned int guidata = 0x558; //windowsize 
		static constexpr unsigned int loopbackpacketsender = 0xF0; //ClientInstance::ClientInstance
	}
}
namespace Index {
	namespace ClientInstance {
		static constexpr unsigned int getRegion = 28;
		static constexpr unsigned int getLocalPlayer = 29;
		static constexpr unsigned int grabMouse = 332;
		static constexpr unsigned int releaseMouse = 333;
	}
}