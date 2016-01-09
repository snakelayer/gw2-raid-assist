#pragma once

#include <sstream>
#include <string>

#include <boost/circular_buffer.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/format.hpp>
#include <boost/timer/timer.hpp>

#include "gw2lib.h"

using namespace boost;
using namespace std;

class RaidBoss {
	public:
		RaidBoss(GW2LIB::Agent &agent);

		float getCurrentHealth() { return agent->GetCharacter().GetCurrentHealth(); }
		int getAgentId() { return agent->GetAgentId(); }
		void updateState();
		void outputDps(stringstream &ss);
		string getOutputHeader() { return outputHeader; }
		int getEncounterDuration();

		virtual string getName() = 0;
		virtual void updateState(boost::circular_buffer<float> &damageBuffer) = 0;
		virtual void outputAssistInfo(stringstream &ss) = 0;

		virtual void outputDebug(stringstream &ss) = 0;

	protected:
		GW2LIB::Agent *agent;

		static int DPS_DURATIONS[3];
		float dps[3];

		boost::timer::cpu_timer encounterTimer;
		int totalEncounterDuration;

		string outputHeader;

		bool hasTakenDamage() { return agent->GetCharacter().GetCurrentHealth() != agent->GetCharacter().GetMaxHealth(); }
		void updateDps(boost::circular_buffer<float> &damageBuffer);

	private:
		float secondsToDeath;
};
