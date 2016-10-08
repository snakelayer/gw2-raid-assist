#pragma once

#include <fstream>

#include <boost/format.hpp>
#include <boost/timer/timer.hpp>

#include "gw2lib.h"

#include "../recordable_stats.h"

class PersonalCombat : public RecordableStats
{
    public:
        PersonalCombat();

        void record(GW2LIB::GW2::CombatLogType type, int amount);
        void reset();

        int getTotalDamageGiven();
        int getTotalDps();

    private:
        int condiDamageTaken;
        int directDamageTaken;
        int healingTaken;

        int condiDamageGiven;
        int directDamageGiven;
        int healingGiven;

        boost::timer::cpu_timer timer;

        void writeToFile();
};