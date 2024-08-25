#pragma once
#include <Windows.h>
#include "../include/ImGui/imgui.h"
#include "ColorUtils.h"
#include <WinUser.h>
#include <rpcasync.h>
#include <windef.h>
#include "../GameData.h"
#include "../SDK/SDK.h"
class ImGuiUtils {
private:
	static float fontSize;
	static ImDrawList* drawList;
public:
	static ImGuiIO GetIO() { return ImGui::GetIO(); };
	static float getDeltaTime() { return GetIO().DeltaTime; };

	static void setDrawList(ImDrawList* drawList) {
		ImGuiUtils::drawList = drawList;
	}

	static vec2 getDisplaySize() {
		RECT drect;
		const HWND h = GetDesktopWindow();
		GetClientRect(h, &drect);
		return vec2{ static_cast<float>(drect.right), static_cast<float>(drect.bottom) };
	}


	static vec2 getScreenResolution() {
		return vec2(GameData::getGuiData()->widthReal, GameData::getGuiData()->heightReal);
	}
	static float getScale() {
		vec2 windowsize = getScreenResolution();
		vec2 basesize = getDisplaySize();
		return min(windowsize.x / basesize.x, windowsize.y / basesize.y);
	}
	static void fillRectangle(vec4 rect, Color col, float round = 0.f) {
		drawList->AddRectFilled(ImVec2(rect.x, rect.y), ImVec2(rect.z, rect.w), col.toImColor(), round * getScale());
	}
	static void drawRectangle(vec4 rect, Color col, float thickness = 1.f, float round = 0.f) {
		drawList->AddRect(ImVec2(rect.x, rect.y), ImVec2(rect.z, rect.w), col.toImColor(), round * getScale());
	}
	static void drawGlowedShape(vec4 rect, Color col, float shadowThickness, float round = 0.f) {
		drawList->AddShadowRect(ImVec2(rect.x, rect.y), ImVec2(rect.z, rect.w), col.toImColor(), shadowThickness, ImVec2(), 0, round * getScale());
	}
	static void drawText(vec2 pos, std::string text, Color textColor, float textSize = 1.f, bool shadow = true, float offset = 1.f, ImFont* font = nullptr) {
		if (shadow) {
			Color shadowColor = textColor;
			shadowColor.r = static_cast<int>(shadowColor.r * 0.2f);
			shadowColor.g = static_cast<int>(shadowColor.g * 0.2f);
			shadowColor.b = static_cast<int>(shadowColor.b * 0.2f);
			
			drawList->AddText(font, fontSize * textSize * getScale(), ImVec2(pos.x + offset * getScale(), pos.y + offset * getScale()), shadowColor.toImColor(), text.c_str());
		}
		drawList->AddText(font, fontSize * textSize * getScale(), ImVec2(pos.x, pos.y), textColor.toImColor(), text.c_str());
	}


	static float getTextWidth(const char* textStr, float textSize, const ImFont* font = nullptr) {
		if (font)
			return font->CalcTextSizeA(textSize * getScale(), FLT_MAX, -1, textStr).x;
		return ImGui::GetFont()->CalcTextSizeA(textSize * getScale(), FLT_MAX, -1, textStr).x;
	}

	static float getTextHeight(float textSize, std::string text, const ImFont* font = nullptr) {
		if (font)
			return font->CalcTextSizeA(textSize * getScale(), FLT_MAX, -1, text.c_str()).y;
		return ImGui::GetFont()->CalcTextSizeA(textSize * getScale(), FLT_MAX, -1, text.c_str()).y;
	}
};