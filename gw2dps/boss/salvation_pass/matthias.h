#pragma once

#include <sstream>

#include "gw2lib.h"

#include "../raid_boss.h"
#include "skills/corruption.h"

class Matthias : public RaidBoss
{
    public:
        static RaidBoss* instance(GW2LIB::Agent agent) { return new Matthias(agent); }
        static bool matchesTarget(GW2LIB::Agent &agent);

        std::string getName() { return "Matthias Gabrel"; }
        void updateState(boost::circular_buffer<float> &damageBuffer);
        void drawAssistInfo();

        void drawCorruptionStatus();

        void outputDebug(std::stringstream &ss);

    private:
        static const float MAX_HP;

        Corruption corruption;
        const float corruptionDisplayOffset = 20.0f;

        Matthias(GW2LIB::Agent agent);

        float getMaxHp() { return MAX_HP; }
        float getBossHeight() { return 280.0f; }

        void drawHealthTicks();
};
