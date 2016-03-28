#pragma once

#include <boost/timer/timer.hpp>

#include "gw2lib.h"

#include "../../../boss/skills/draw/meter.h"
#include "../../../assist_drawer.h"

class Sacrifice
{
    public:
        Sacrifice();

        void updateState(GW2LIB::Agent bossAgent);

        int getRemainingDuration();

        void draw();

    private:
        static const int DURATION = 10 * 1000;
        static const float PLAYER_HEIGHT_OFFSET;

        boost::timer::cpu_timer timer;
        GW2LIB::Agent sacrificeTarget;

        Meter meter;

        bool isSelfSacrifice(GW2LIB::Agent bossAgent);
        bool isAllySacrifice(GW2LIB::Agent allyAgent);
        void setTarget(GW2LIB::Agent agent);
        void clearTarget();

        void updateSacrificeTarget(GW2LIB::Agent bossAgent);
};