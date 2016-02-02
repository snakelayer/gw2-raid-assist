#pragma once

#include <fstream>
#include <map>
#include <ostream>
#include <sstream>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/format.hpp>

#include "GameData.h"
#include "gw2lib.h"

#include "../commonTypedefs.h"
#include "../boss/raid_boss.h"
#include "squad_member.h"

using namespace boost;
using namespace std;

class RaidBoss;

namespace RAID {
	enum RaidState {
		ACTIVE = 0, // at least one player is alive
		DOWNED, // everyone is downed or dead
		DEAD // everyone is dead
	};
}

class Squad
{
	private:
		static const int MAX_SQUAD_SIZE = 10;
		SquadMemberMap members;
		RaidBoss *raidBoss;

		RAID::RaidState raidState;
		bool turnOffWhenRespawn;
		static const string logFile;

		int outputMask;
		bool shouldOutputDodges();
		bool shouldOutputTotalDamageTaken();
		void updateRaidState(CharacterMap &characterMap);
		void writeStatsToFile();

		bool hasPlayerWithName(string name);

		string debugStr;

	public:
		string getDebugStr() { return debugStr; }
		Squad();
		~Squad();
		
		void setBoss(RaidBoss *raidBoss);
		void addPlayer(GW2LIB::Character character);
		CharacterMap getCharacterMap();
		void updateState();
		void updateDodgeState(CharacterSpeeds &characterSpeeds);

		void incrementDisplayMask();
		void outputPlayerStats(ostream &stream);

		RAID::RaidState getRaidState() { return raidState; }
		bool turnOff() { return turnOffWhenRespawn && GW2LIB::GetOwnCharacter().IsAlive(); }
		string getLogFileName() { return logFile; }
};
