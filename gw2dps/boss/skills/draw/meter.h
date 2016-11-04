#pragma once

#include <string>

#include <boost/format.hpp>

#include "gw2lib.h"

#include "../../../assist_drawer.h"

class Meter
{
    public:
        Meter();
        Meter(float width, float height);

        void drawAtPercent(float x, float y, DWORD color, float percent);
        void drawAtPercent(float x, float y, DWORD color, std::string text, float percent);
        void drawAtPercent(float x, float y, DWORD color, float cooldown, float percent);
        void drawAtAgentPositionWithZOffset(GW2LIB::Agent &agent, float zOffset, DWORD color, float percent);

    private:
        static const float DEFAULT_WIDTH;
        static const float DEFAULT_HEIGHT;
        float width;
        float height;
};