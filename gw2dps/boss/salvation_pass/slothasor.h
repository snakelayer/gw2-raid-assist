#pragma once

#include "gw2lib.h"

#include "../raid_boss.h"

class Slothasor : public RaidBoss
{
    public:
        static RaidBoss* instance(GW2LIB::Agent agent) { return new Slothasor(agent); }
        static bool matchesTarget(GW2LIB::Agent &agent);

        float getBossHeight() { return 300.0f; } // TODO: tweak

        std::string getName() { return "Slothasor"; }
        void updateState(boost::circular_buffer<float> &damageBuffer);
        void drawAssistInfo();

        void outputDebug(std::stringstream &ss);
    private:
        static const float MAX_HP;

        Slothasor(GW2LIB::Agent agent);

        float getMaxHp() { return MAX_HP; }
};
