#pragma once

#include <cmath>
#include <map>
#include <sstream>

#include <boost/assign.hpp>
#include <boost/format.hpp>
#include <boost/timer/timer.hpp>

#include "gw2lib.h"

#include "../../skills/draw/meter.h"
#include "../../../assist_drawer.h"

namespace CORRUPTION {
    enum DIRECTION {
        SW = 0,
        NW,
        NE,
        SE
    };
}

class Corruption {
    public:
        Corruption();

        void updateState(int elapsedMilliseconds);

        void drawCleansingFonts();
        void drawMeter(float x, float y);

    private:
        static const int FONT_COOLDOWN = 90;
        static const int SKILL_COOLDOWN = 30 * 1000;
        static const int SKILL_START_OFFSET = 25 * 1000;
        static const float FONT_RADIUS;
        static std::map<CORRUPTION::DIRECTION, GW2LIB::Vector3> positionMap;

        int elapsedMilliseconds;
        std::map<CORRUPTION::DIRECTION, boost::timer::cpu_timer> cooldownMap;

        Meter meter;

        bool isInFont(CORRUPTION::DIRECTION direction, GW2LIB::Vector3 pos);
        bool isFontReady(CORRUPTION::DIRECTION direction);
        int getFontCooldown(CORRUPTION::DIRECTION direction);

        float getSkillCooldown();

        void updateCleansingFonts();
};