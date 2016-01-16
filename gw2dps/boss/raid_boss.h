#pragma once

#include <sstream>
#include <string>

#include <boost/circular_buffer.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/format.hpp>
#include <boost/timer/timer.hpp>

#include "gw2lib.h"

#include "../raid/squad.h"

class Squad;

using namespace boost;
using namespace std;
using namespace GW2LIB;

class RaidBoss {
	public:
		RaidBoss(Agent agent);

		void setLogFile(string file) { logFile = file; }
		void updateState();
		void outputDps(stringstream &ss);
		string getOutputHeader() { return outputHeader; }
		int getEncounterDuration();

		virtual string getName() = 0;
		virtual void updateState(boost::circular_buffer<float> &damageBuffer) = 0;
		virtual void updateSquadState(Squad *squad) = 0;
		virtual void outputAssistInfo(stringstream &ss) = 0;

		virtual void outputDebug(stringstream &ss) = 0;

		bool isDead() { return !agent.GetCharacter().IsAlive(); }
		int getAgentId() { return agent.GetAgentId(); }
		float getCurrentHealth() { return agent.GetCharacter().GetCurrentHealth(); }
		float getBreakbar() { return agent.GetCharacter().GetBreakbarPercent(); }
		GW2::BreakbarState breakbarState() { return agent.GetCharacter().GetBreakbarState(); }

	protected:
		Agent agent;

		static int DPS_DURATIONS[3];
		float dps[3];

		boost::timer::cpu_timer encounterTimer;
		int totalEncounterDuration;

		string outputHeader;

		bool hasTakenDamage() { return agent.GetCharacter().GetCurrentHealth() != agent.GetCharacter().GetMaxHealth(); }
		void outputAssistHeader(stringstream &ss);
		void updateDps(boost::circular_buffer<float> &damageBuffer);

	private:
		string logFile;

		vector<float> remainingHealth;
		float secondsToDeath;

		float dist(Vector3 p1, Vector3 p2);
		void writeDataToFile();
		void writeHealthData(ostream &stream);
};
