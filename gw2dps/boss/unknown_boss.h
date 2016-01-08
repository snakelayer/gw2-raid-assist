#ifndef UNKNOWN_BOSS_H
#define UNKNOWN_BOSS_H

#include <sstream>

#include <boost/circular_buffer.hpp>
#include <boost/format.hpp>

#include "raid_boss.h"

using namespace boost;
using namespace GW2LIB;

class UnknownBoss : public RaidBoss
{
	private:

	public:
		UnknownBoss(GW2LIB::Agent &agent);

		string getName() { return "Unknown"; }
		bool matchesTarget(GW2LIB::Agent &agent) { return false; }

		void updateState(boost::circular_buffer<float> &damageBuffer);
		void outputDps(stringstream &ss);
		void outputAssistInfo(stringstream &ss);
		int getEncounterTime();

		void outputDebug(stringstream &ss) {}
};

#endif