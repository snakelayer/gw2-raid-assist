#include "meter.h"

using namespace GW2LIB;
using namespace std;

const float Meter::DEFAULT_WIDTH = 160.0f;
const float Meter::DEFAULT_HEIGHT = 20.0f;

Meter::Meter() : width(DEFAULT_WIDTH), height(DEFAULT_HEIGHT) {
}

Meter::Meter(float width, float height) : width(width), height(height) {
}

void Meter::draw(float x, float y, DWORD color, string text) {
    drawAtPercent(x, y, color, text, 1.0f);
}

void Meter::drawAtPercent(float x, float y, DWORD color, string text, float percent) {
    DrawRectFilled(x - width / 2, y, width * percent, height, color);
    if (!text.empty()) {
        AssistDrawer::get().drawFont(x - width / 8, y + AssistDrawer::PADY, AssistDrawer::WHITE, text); // TODO: check width/8
    }
    DrawRect(x - width / 2, y, width, height, AssistDrawer::BORDER_COLOR);
}
