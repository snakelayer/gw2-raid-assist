#pragma once

#include <sstream>

#include "gw2lib.h"

#include "../raid_boss.h"

class Matthias : public RaidBoss
{
    public:
        static RaidBoss* instance(GW2LIB::Agent agent) { return new Matthias(agent); }
        static bool matchesTarget(GW2LIB::Agent &agent);

        std::string getName() { return "Matthias Gabrel"; }
        void updateState(boost::circular_buffer<float> &damageBuffer);
        void drawAssistInfo();

        void outputDebug(std::stringstream &ss);

    private:
        static const float MAX_HP;

        Matthias(GW2LIB::Agent agent);

        float getMaxHp() { return MAX_HP; }
        float getBossHeight() { return 280.0f; }

        void drawHealthTicks();
};
