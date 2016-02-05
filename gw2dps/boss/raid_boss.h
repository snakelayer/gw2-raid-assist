#pragma once

#include <fstream>
#include <sstream>
#include <string>

#include <boost/circular_buffer.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/format.hpp>

#include "gw2lib.h"

#include "../assist_drawer.h"
#include "../common_typedefs.h"
#include "../raid/encounter_timer.h"

class RaidBoss {
	public:
		RaidBoss(GW2LIB::Agent agent);

		void updateState();
		void outputDps(std::stringstream &ss);
		std::string getOutputHeader() { return outputHeader; }
		void reset();

		virtual std::string getName() = 0;
		virtual void updateState(boost::circular_buffer<float> &damageBuffer) = 0;
		virtual void updateSquadState(SquadMemberMap &members) = 0;
		virtual void drawAssistInfo() = 0;

		virtual void outputDebug(std::stringstream &ss) = 0;

		bool isDead() { return !agent.GetCharacter().IsAlive(); }
		int getAgentId() { return agent.GetAgentId(); }
		float getCurrentHealth() { return agent.GetCharacter().GetCurrentHealth(); }
		float getBreakbar() { return agent.GetCharacter().GetBreakbarPercent(); }
		GW2LIB::GW2::BreakbarState breakbarState() { return agent.GetCharacter().GetBreakbarState(); }

		bool getScreenLocation(float *x, float *y);

	protected:
		GW2LIB::Agent agent;

		static int DPS_DURATIONS[3];
		float dps[3];

		EncounterTimer encounterTimer;
		int totalEncounterDuration;

		std::string outputHeader;

		virtual float getMaxHp() = 0;
		bool tryResetBossAgent();
		bool hasTakenDamage() { return agent.GetCharacter().GetCurrentHealth() != agent.GetCharacter().GetMaxHealth(); }
		void drawToWindow(std::stringstream &ss);
		void outputAssistHeader(std::stringstream &ss);

		void updateDps(boost::circular_buffer<float> &damageBuffer);

	private:
		static const std::string logFile;

		std::vector<float> remainingHealth;
		float secondsToDeath;

		const float X_BUFFER = 100.0f;
		const float Y_BUFFER = 50.0f;
		float lastX;
		float lastY;

		float dist(GW2LIB::Vector3 p1, GW2LIB::Vector3 p2);
		void writeDataToFile();
		void writeHealthData(std::ostream &stream);
};
