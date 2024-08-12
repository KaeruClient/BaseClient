#pragma once
#include <Windows.h>
#include "../../Utils/Utils.h"
namespace Signature {
	static const uintptr_t keyboard_feed = SigScan("48 83 ec ? 0f b6 c1 4c 8d 05");

}