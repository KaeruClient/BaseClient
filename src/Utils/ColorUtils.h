#pragma once
#include "Math.h"
#include "../include/ImGui/imgui.h"
#include <string>
struct MCColor {
	union {
		struct { float r, g, b, a; };
		float arr[4]{};
	};
	bool shouldDelete = true;

	MCColor() {
		this->r = 1;
		this->g = 1;
		this->b = 1;
		this->a = 1;
	}

	MCColor(float x) {
		this->r = x;
		this->g = x;
		this->b = x;
		this->a = 1;
	}

	MCColor(const MCColor& other) {
		this->r = other.r;
		this->g = other.g;
		this->b = other.b;
		this->a = other.a;
		this->shouldDelete = other.shouldDelete;
	}

	MCColor(const float r, const float g, const float b, const float a = 255) {
		this->r = r / 255.0f;
		this->g = g / 255.0f;
		this->b = b / 255.0f;
		this->a = a / 255.0f;
	}

	MCColor(const int r, const int g, const int b, const int a = 255) {
		this->r = r / 255.0f;
		this->g = g / 255.0f;
		this->b = b / 255.0f;
		this->a = a / 255.0f;
	}

	MCColor(const float r, const float g, const float b, const float a, const bool shouldDelete) {
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
		this->shouldDelete = shouldDelete;
	}

	// Type Conversions

	operator ImVec4() const {
		return ImVec4(r, g, b, a);
	}

	operator ImColor() const {
		return ImColor(r, g, b, a);
	}

	operator ImU32() const {
		return ImGui::ColorConvertFloat4ToU32(ImVec4(r, g, b, a));
	}

	// Custom Functions

	// ToHSV

	vec3<float> GetHSV() {
		float h, s, v;
		ImGui::ColorConvertHSVtoRGB(r, g, b, h, s, v);
		return vec3<float>(h, s, v);
	}

	std::string ToString() {
		return std::to_string(r) + ", " + std::to_string(g) + ", " + std::to_string(b) + ", " + std::to_string(a);
	}
};

struct Color {
	int r = 255;
	int g = 255;
	int b = 255;
	int a = 255;
	Color(int red, int green, int blue, int alpha = 255) {
		this->r = red;
		this->g = green;
		this->b = blue;
		this->a = alpha;
	}
	MCColor toMCColor() {
		return MCColor((float)this->r / 255.f, (float)this->g / 255.f, (float)this->b / 255.f, (float)this->a / 255.f);
	}
	ImColor toImColor() {
		return ImColor(this->r, this->g, this->b, this->a);
	}
};
