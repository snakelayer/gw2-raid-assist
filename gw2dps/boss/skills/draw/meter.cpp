#include "meter.h"

using namespace GW2LIB;
using namespace std;

const float Meter::DEFAULT_WIDTH = 160.0f;
const float Meter::DEFAULT_HEIGHT = 16.0f;

Meter::Meter() : width(DEFAULT_WIDTH), height(DEFAULT_HEIGHT) {
}

Meter::Meter(float width, float height) : width(width), height(height) {
}

/*void Meter::draw(float x, float y, DWORD color, string text) {
    drawAtPercent(x, y, color, text, 1.0f);
}*/

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
