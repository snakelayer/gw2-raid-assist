#pragma once

#include <fstream>
#include <map>
#include <ostream>
#include <sstream>

#include <boost/format.hpp>

#include "GameData.h"
#include "gw2lib.h"

#include "../common_typedefs.h"
#include "../recordable_stats.h"
#include "../boss/raid_boss.h"
#include "squad_member.h"

class RaidBoss;

namespace RAID {
    enum RaidState {
        ACTIVE = 0, // at least one player is alive
        DOWNED, // everyone is downed or dead
        DEAD // everyone is dead
    };
}

class Squad : public RecordableStats
{
    private:
        static const int MAX_SQUAD_SIZE = 10;
        SquadMemberMap members;
        RaidBoss *raidBoss;

        RAID::RaidState raidState;
        bool disable;

        void addPlayer(GW2LIB::Player player);

        void updateHeavyHits(float heavyHitDamageThreshold);
        void updateRaidState(CharacterMap &characterMap);

        void writeToFile();

        std::string debugStr;

    public:
        std::string getDebugStr() { return debugStr; }
        Squad();
        ~Squad();
        
        void setBoss(RaidBoss *raidBoss);
        CharacterMap getCharacterMap();
        void updateState();
        void updateDamage(GW2LIB::Agent agent, int damage);

        void outputPlayerStats(std::ostream &stream);

        RAID::RaidState getRaidState() { return raidState; }
        bool turnOff() { return disable; }
};
