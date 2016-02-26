#pragma once

#include <map>

#include <boost/assign.hpp>
#include <boost/format.hpp>
#include <boost/timer/timer.hpp>

#include "gw2lib.h"

#include "draw/meter.h"
#include "../../assist_drawer.h"

namespace GR {
    enum State
    {
        PENDING = 0,
        INITIAL,
        AFTER_FIRST_HIT,
        AFTER_SECOND_HIT,
        AFTER_THIRD_HIT,
        AFTER_FOURTH_HIT,
        AFTER_FIFTH_HIT
    };
}

class GhastlyRampage
{
    public:
        GhastlyRampage();

        GR::State getState() { return state; }

        void update(GW2LIB::GW2::BreakbarState state, float breakbar);

        void draw(float x, float y);

    private:
        static std::map<GR::State, float> MILLISECONDS_UNTIL_HIT_MAP;
        static std::map<GR::State, GR::State> NEXT_STATE_MAP;

        GR::State state;

        boost::timer::cpu_timer timer;

        Meter meter;

        void turnOff();
        void turnOn();

        float getElapsedMs();
};