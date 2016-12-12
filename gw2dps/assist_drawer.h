#pragma once

#include <sstream>

#include "gw2lib.h"

class AssistDrawer
{
    private:
        static const DWORD BACKGROUND_COLOR = 0xdd000000;

        GW2LIB::Font font;

        AssistDrawer();
        AssistDrawer(AssistDrawer const&) = delete;
        AssistDrawer& operator=(AssistDrawer const&) = delete;

    public:
        static const int PADX = 5;
        static const int PADY = 2;
        static const int lineHeight = 16;

        static const DWORD FLOAT_MASK = 0x30000000;
        static const DWORD BG_COLOR_MASK = 0x33000000;

        static const DWORD BORDER_COLOR = 0xff444444;
        static const DWORD BLACK = 0xff000000;
        static const DWORD WHITE = 0xffffffff;
        static const DWORD RED = 0xffff0000;
        static const DWORD CYAN = 0xff66ffff;

        static const DWORD HEALTHBAR_TICK = 0xff60e0e0;
        static const DWORD HEALTHBAR_TICK2 = 0xff1010a0;
        static const DWORD METER_BACKGROUND = 0x60000000;
        static const DWORD BREAKBAR_FULL = 0xff418278;
        static const DWORD BREAKBAR_DEPLETING = 0xff418278;
        static const DWORD BREAKBAR_RECHARGING = 0Xffa06841;
        static const DWORD SKILL_RECHARGING = BREAKBAR_RECHARGING; // TODO: use something else

        static AssistDrawer& get()
        {
            static AssistDrawer instance;
            return instance;
        }

        static GW2LIB::Vector2 StringInfo(std::string str);

        static float adjustXForNextElement(float x) {
            return x - PADX * 3;
        }

        static float adjustYForNextElement(float y) {
            return y + PADY * 2;
        }

        void drawBackground(float x, float y, GW2LIB::Vector2 strInfo);
        void drawFont(float x, float y, DWORD color, std::string format, ...);
        void drawFont(GW2LIB::Agent &agent, DWORD color, std::string format, ...);
        void drawStreamToWindow(std::stringstream &ss, float x, float y);
};