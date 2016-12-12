#include "assist_drawer.h"

using namespace GW2LIB;
using namespace std;

AssistDrawer::AssistDrawer() {
    font.Init(AssistDrawer::lineHeight, "Verdana");
}

Vector2 AssistDrawer::StringInfo(string str) {
    return AssistDrawer::get().font.TextInfo(str);
}

void AssistDrawer::drawBackground(float x, float y, Vector2 strInfo) {
    DrawRectFilled(x - PADX, y - PADY, strInfo.x + PADX * 2, strInfo.y + PADY * 2, BACKGROUND_COLOR - BG_COLOR_MASK); //black background
    DrawRect(x - PADX, y - PADY, strInfo.x + PADX * 2, strInfo.y + PADY * 2, BORDER_COLOR); // white border
}

void AssistDrawer::drawFont(float x, float y, DWORD color, string format, ...) {
    va_list vl;
    va_start(vl, format);
    font.Draw(x, y, color, format, vl);
    va_end(vl);
}

void AssistDrawer::drawFont(Agent &agent, DWORD color, string format, ...) {
    float x, y;
    if (WorldToScreen(agent.GetPos(), &x, &y)) {
        va_list vl;
        va_start(vl, format);
        font.Draw(x, y, color, format, vl);
        va_end(vl);
    }
}

void AssistDrawer::drawStreamToWindow(stringstream &ss, float x, float y) {
    Vector2 strInfo = font.TextInfo(ss.str());
    float adjusted_x = round(x - strInfo.x / 2);

    DrawRectFilled(adjusted_x - PADX, y - PADY, strInfo.x + PADX * 2, strInfo.y + PADY * 2, BACKGROUND_COLOR); //black background
    DrawRect(adjusted_x - PADX, y - PADY, strInfo.x + PADX * 2, strInfo.y + PADY * 2, BORDER_COLOR); // white border
    drawFont(adjusted_x, y, WHITE, ss.str());
}
