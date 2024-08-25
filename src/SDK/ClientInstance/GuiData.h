#pragma once
#include "../../Utils/Math.h"
class GuiData {
private:
	char pad_0x0000[0x30];  //0x0000
public:
	union {
		struct {
			float widthReal;   //0x0018
			float heightReal;  //0x001C
		};
		vec2 windowSizeReal;  //0x0018
	};

	float widthReal2;   //0x0020
	float heightReal2;  //0x0024
	union {
		struct {
			float widthGame;   //0x0028
			float heightGame;  //0x002C
		};
		vec2 windowSize;  //0x0028
	};
private:
	char pad_0x0048[0x4]; //0x0048
public:
	float GuiScale; //0x004C 
};
