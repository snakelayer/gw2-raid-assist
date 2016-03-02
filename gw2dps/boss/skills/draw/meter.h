#pragma once

#include <string>

#include "gw2lib.h"

#include "../../../assist_drawer.h"

class Meter
{
    public:
        Meter();
        Meter(float width, float height);

        void draw(float x, float y, DWORD color, std::string text);
        void drawAtPercent(float x, float y, DWORD color, std::string text, float percent);

    private:
        static const float DEFAULT_WIDTH;
        static const float DEFAULT_HEIGHT;
        float width;
        float height;
};