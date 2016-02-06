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

		float getMaxHp() { return agent.GetCharacter().GetMaxHealth(); }
		float getBossHeight() { return 300.0f; }

		std::string getName() { return "Unknown"; }
		static bool matchesTarget(GW2LIB::Agent &agent) { return false; }

		void updateState(boost::circular_buffer<float> &damageBuffer);
		void updateSquadState(SquadMemberMap &members);
		void outputDps(std::stringstream &ss);
		void drawAssistInfo();
		int getEncounterTime();

		void outputDebug(std::stringstream &ss);
};
