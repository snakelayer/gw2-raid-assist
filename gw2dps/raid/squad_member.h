#pragma once

#include <boost/timer/timer.hpp>

#include "GameData.h"

using namespace std;

namespace SM {

	enum SPEED_STATE {
		BASIC = 0, // up to 200
		SWIFTNESS, // up to 266
		DODGE, // set to 366
		SUPERSPEED // up to 400
	};
}

class SquadMember {
	private:
		static const float SWIFTNESS_THRESHOLD;
		static const float DODGE_THRESHOLD;
		static const float SUPERSPEED_THRESHOLD;

		string name;
		int dodgeCount;
		int hitsTaken;
		float totalDamageTaken;

		SM::SPEED_STATE speedState;
		boost::timer::cpu_timer dodgeTimer;
		bool isAlive;
		float lastEndurance;
		float lastHealth;
		float lastHealthDelta;

		void updateLastHealthDelta(GW2LIB::Character &character);
		void updateDamageTaken();
		void updateDodgeCount(GW2LIB::Character &character);
		void tryUpdateDodgeCount();

		SquadMember() = delete;

	public:
		SquadMember(GW2LIB::Character character);
		void updateStats(GW2LIB::Character &character);
		void inferDodgeStateWithSpeed(float speed);
		void reset();

		string getName() { return name; }
		int getDodgeCount() { return dodgeCount; }
		float getLastHealthDelta() { return lastHealthDelta; }
		int getHitsTaken() { return hitsTaken; }
		float getTotalDamageTaken() { return totalDamageTaken; }

		void incrementHitsTaken() { if (isAlive) { ++hitsTaken; } }

		SM::SPEED_STATE getSpeedState() { return speedState; }
};
