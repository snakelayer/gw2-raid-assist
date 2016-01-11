#pragma once

#include "GameData.h"

using namespace std;

class SquadMember {
	private:
		string name;
		int dodgeCount;
		int hitsTaken;
		float totalDamageTaken;

		float lastEndurance;
		float lastHealth;
		float lastHealthDelta;

		void updateLastHealthDelta(GW2LIB::Character &character);
		void updateDamageTaken();
		void updateDodgeCount(GW2LIB::Character &character);

	public:
		SquadMember(GW2LIB::Character character);
		void updateStats(GW2LIB::Character &character);
		void reset();

		string getName() { return name; }
		int getDodgeCount() { return dodgeCount; }
		float getLastHealthDelta() { return lastHealthDelta; }
		int getHitsTaken() { return hitsTaken; }
		float getTotalDamageTaken() { return totalDamageTaken; }

		void incrementHitsTaken() { ++hitsTaken; }
};
