#pragma once

#define _USE_MATH_DEFINES // for pi

#include <math.h>

#include <boost/assign.hpp>
#include <boost/timer/timer.hpp>

#include "gw2lib.h"

#include "../../../assist_drawer.h"

namespace UP {
    enum State
    {
        PENDING_SPLIT = 0,
        PENDING_MOVE_FIRST,
        SAFE_RED_BLUE,
        SAFE_GREEN_RED,
        SAFE_BLUE_GREEN,
        PENDING_MOVE_SECOND,
        SAFE_RED,
        SAFE_GREEN,
        SAFE_BLUE
    };
}

class UnstablePylon
{
    public:
        UnstablePylon();

        void startWith(UP::State state);
        void update();
        void stop();

        void draw();

    private:
        const float FIRST_COOLDOWN = 20.0f;
        const float SECOND_COOLDOWN = 17.0f;
        const float MOVE_COOLDOWN = 4.0f;

        static std::map<UP::State, float> COLOR_ANGLE_MAP;
        static const float TICK_ANGLE;
        const GW2LIB::Vector3 CENTER = GW2LIB::Vector3(-4775.0f, -20630.0f, -2402.0f);
        const float RADIUS = 1530.0f;

        UP::State state;
        boost::timer::cpu_timer timer;

        bool isFirst();
        bool isSecond();

        void tick();
        float getFirstCooldown();
        float getSecondCooldown();
        float getMoveCooldown();
};
