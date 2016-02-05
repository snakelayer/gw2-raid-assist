#pragma once

#include <sstream>

#include <boost/circular_buffer.hpp>
#include <boost/format.hpp>

#include "raid_boss.h"

class UnknownBoss : public RaidBoss
{
	private:
		std::string debugStr;

	public:
		UnknownBoss(GW2LIB::Agent agent);

		float getMaxHp() { return 0; }

		std::string getName() { return "Unknown"; }
		static bool matchesTarget(GW2LIB::Agent &agent) { return false; }

		void updateState(boost::circular_buffer<float> &damageBuffer);
		void updateSquadState(SquadMemberMap &members);
		void outputDps(std::stringstream &ss);
		void outputAssistInfo(std::stringstream &ss);
		int getEncounterTime();

		void outputDebug(std::stringstream &ss);
};
