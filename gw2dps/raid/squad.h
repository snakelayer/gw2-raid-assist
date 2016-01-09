#pragma once

#include <fstream>
#include <map>
#include <ostream>
#include <sstream>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/format.hpp>

#include "GameData.h"
#include "gw2lib.h"

#include "../boss/raid_boss.h"
#include "squad_member.h"

using namespace boost;
using namespace std;

namespace RAID {
	enum RaidState {
		ACTIVE = 0, // at least one player is alive
		DOWNED, // everyone is downed or dead
		DEAD // everyone is dead
	};
}

typedef map<string, GW2LIB::Character> CharacterMap;
typedef pair<string, GW2LIB::Character> CharacterEntry;
typedef map<string, SquadMember> SquadMemberMap;
typedef pair<string, SquadMember> SquadMemberEntry;

class Squad
{
	private:
		static const int MAX_SQUAD_SIZE = 10;
		SquadMemberMap members;
		RaidBoss *raidBoss;

		RAID::RaidState raidState;
		bool resetStateAtNextRespawn;
		string logFile;

		int outputMask;
		CharacterMap getCharacterMap();
		bool shouldOutputDodges();
		bool shouldOutputTotalDamageTaken();
		void updateRaidState(CharacterMap &characterMap);
		void tryReset(CharacterMap &characterMap);
		void writeStatsToFile();

		bool hasPlayerWithName(string name);

		string debugStr;
	public:
		string getDebugStr() { return debugStr; }
		Squad(string logFile = "");
		~Squad();
		
		void setBoss(RaidBoss *raidBoss);
		void addPlayer(GW2LIB::Character character);
		void updateState();

		void incrementDisplayMask();
		void outputPlayerStats(ostream &stream);

		RAID::RaidState getRaidState() { return raidState; }
		bool isResetAtSpawn() { return resetStateAtNextRespawn; }
		string getLogFileName() { return logFile; }
};
