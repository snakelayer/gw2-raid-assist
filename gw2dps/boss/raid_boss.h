#pragma once

#include <fstream>
#include <sstream>
#include <string>

#include <boost/circular_buffer.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/format.hpp>

#include "gw2lib.h"

#include "../commonTypedefs.h"
#include "../raid/encounter_timer.h"


using namespace boost;
using namespace std;
using namespace GW2LIB;

class RaidBoss {
	public:
		RaidBoss(Agent agent);

		void updateState();
		void outputDps(stringstream &ss);
		string getOutputHeader() { return outputHeader; }
		void reset();

		virtual string getName() = 0;
		virtual void updateState(boost::circular_buffer<float> &damageBuffer) = 0;
		virtual void updateSquadState(SquadMemberMap &members) = 0;
		virtual void outputAssistInfo(stringstream &ss) = 0;

		virtual void outputDebug(stringstream &ss) = 0;

		bool isDead() { return !agent.GetCharacter().IsAlive(); }
		int getAgentId() { return agent.GetAgentId(); }
		float getCurrentHealth() { return agent.GetCharacter().GetCurrentHealth(); }
		float getBreakbar() { return agent.GetCharacter().GetBreakbarPercent(); }
		GW2::BreakbarState breakbarState() { return agent.GetCharacter().GetBreakbarState(); }

		bool getScreenLocation(float *x, float *y);

	protected:
		Agent agent;

		static int DPS_DURATIONS[3];
		float dps[3];

		EncounterTimer encounterTimer;
		int totalEncounterDuration;

		string outputHeader;

		bool hasTakenDamage() { return agent.GetCharacter().GetCurrentHealth() != agent.GetCharacter().GetMaxHealth(); }
		void outputAssistHeader(stringstream &ss);
		void updateDps(boost::circular_buffer<float> &damageBuffer);

	private:
		static const string logFile;

		vector<float> remainingHealth;
		float secondsToDeath;

		const float X_BUFFER = 100.0f;
		const float Y_BUFFER = 50.0f;
		float lastX;
		float lastY;

		float dist(Vector3 p1, Vector3 p2);
		void writeDataToFile();
		void writeHealthData(ostream &stream);
};
