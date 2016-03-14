#include "meter.h"

using namespace boost;
using namespace GW2LIB;
using namespace std;

const float Meter::DEFAULT_WIDTH = 160.0f;
const float Meter::DEFAULT_HEIGHT = 16.0f;

Meter::Meter() : width(DEFAULT_WIDTH), height(DEFAULT_HEIGHT) {
}

Meter::Meter(float width, float height) : width(width), height(height) {
}

void Meter::drawAtPercent(float x, float y, DWORD color, float percent) {
    DrawRectFilled(x - width / 2, y, width, height, AssistDrawer::METER_BACKGROUND);
    DrawRectFilled(x - width / 2, y, width * percent, height, color);
    DrawRect(x - width / 2, y, width, height, AssistDrawer::BORDER_COLOR);
}

void Meter::drawAtPercent(float x, float y, DWORD color, string text, float percent) {
    drawAtPercent(x, y, color, percent);
    if (!text.empty()) {
        AssistDrawer::get().drawFont(x - AssistDrawer::PADX * 2, y, AssistDrawer::WHITE, text);
    }
}

void Meter::drawAtPercent(float x, float y, DWORD color, float cooldown, float percent) {
    string cooldownStr;
    if (cooldown < 5.0f) {
        cooldownStr = str(format("%.1f") % cooldown);
    }
    else {
        cooldownStr = str(format("%d") % int(cooldown));
    }

    drawAtPercent(x, y, color, cooldownStr, percent);
}
