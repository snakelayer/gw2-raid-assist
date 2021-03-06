#pragma once

#include <sstream>

#include <boost/circular_buffer.hpp>
#include <boost/format.hpp>

#include "raid_boss.h"

class UnknownBoss : public RaidBoss
{
    private:

    public:
        UnknownBoss(GW2LIB::Agent agent);

        float getMaxHp() { return agent.GetCharacter().GetMaxHealth(); }
        float getBossHeight() { return 200.0f; }

        std::string getName() { return "Unknown Boss"; }
        static bool matchesTarget(GW2LIB::Agent &agent) { return false; }

        void updateState(boost::circular_buffer<float> &damageBuffer);
        void drawAssistInfo();
        int getEncounterTime();

        void outputDebug(std::stringstream &ss);
};
