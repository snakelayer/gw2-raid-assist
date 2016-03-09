#pragma once

#include "gw2lib.h"

#include "../raid_boss.h"
#include "skills/volatile_poison.h"

class Slothasor : public RaidBoss
{
    public:
        static RaidBoss* instance(GW2LIB::Agent agent) { return new Slothasor(agent); }
        static bool matchesTarget(GW2LIB::Agent &agent);

        float getBossHeight() { return 300.0f; }

        std::string getName() { return "Slothasor"; }
        void updateState(boost::circular_buffer<float> &damageBuffer);
        void drawAssistInfo();

        void outputDebug(std::stringstream &ss);
    private:
        static const float MAX_HP;

        VolatilePoison vp;

        Slothasor(GW2LIB::Agent agent);

        float getMaxHp() { return MAX_HP; }
        bool hasPlayerSlubling();

        const float volatilePoisonDisplayOffset = 40.0f;
        void updateVolatilePoison();
        void drawVolatilePoisonStatus();
};
