#include "ArrayList.h"

ArrayList::ArrayList() : Module("ArrayList", Category::Render, 0x0, "temp arraylist.") {
}

void ArrayList::onPresentEvent(ImDrawList* d) {
	float yOffset = 0.f;
	for (auto& mod : modules.getEnabledList()) {
		ImGuiUtils::drawText(vec2(0.f, yOffset), mod->getName(), Color(255,255,255), 1.f);
		yOffset += ImGuiUtils::getTextHeight(1.f);
	}
}