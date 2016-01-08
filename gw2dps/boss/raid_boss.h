#ifndef RAID_BOSS_H
#define RAID_BOSS_H

#include <sstream>
#include <string>

#include <boost/circular_buffer.hpp>
#include <boost/timer/timer.hpp>

#include "gw2lib.h"

using namespace std;

class RaidBoss {
	public:
		virtual string getName() = 0;
		virtual float getCurrentHealth() = 0;
		virtual int getAgentId() = 0;
		virtual bool matchesTarget(GW2LIB::Agent &agent) = 0;

		virtual void updateState(boost::circular_buffer<float> &damageBuffer) = 0;
		virtual void outputDps(stringstream &ss) = 0;
		virtual void outputAssistInfo(stringstream &ss) = 0;
		virtual int getEncounterTime() = 0;

		virtual void outputDebug(stringstream &ss) = 0;

	protected:
		static int DPS_DURATIONS[3];

		boost::timer::cpu_timer encounterTimer;
};

#endif
