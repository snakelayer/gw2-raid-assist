#pragma once

#include <sstream>

#include "gw2lib.h"

#include "../raid_boss.h"
#include "skills/corruption.h"
#include "skills/sacrifice.h"

namespace MATTHIAS {
    enum Phase {
        FIRST = 0,
        SECOND,
        THIRD,
        LAST
    };
}

class Matthias : public RaidBoss
{
    public:
        static RaidBoss* instance(GW2LIB::Agent agent) { return new Matthias(agent); }
        static bool matchesTarget(GW2LIB::Agent &agent);

        std::string getName() { return "Matthias Gabrel"; }
        void updateState(boost::circular_buffer<float> &damageBuffer);
        void drawAssistInfo();

        void drawCorruptionStatus();
        void drawSacrificeStatus();

        void outputDebug(std::stringstream &ss);

    private:
        static const float MAX_HP;

        MATTHIAS::Phase phase;

        Corruption corruption;
        const float corruptionDisplayOffset = 20.0f;

        Sacrifice sacrifice;

        Matthias(GW2LIB::Agent agent);

        float getMaxHp() { return MAX_HP; }
        float getBossHeight() { return 280.0f; }

        void drawHealthTicks();
};
