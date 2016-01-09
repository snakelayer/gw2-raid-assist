#pragma once

#include "GameData.h"

using namespace std;

class SquadMember {
	private:
		string name;
		int dodgeCount;
		float totalDamageTaken;

		float lastEndurance;
		float lastHealth;

		void updateTotalDamageTaken(GW2LIB::Character &character);
		void updateDodgeCount(GW2LIB::Character &character);

	public:
		SquadMember(GW2LIB::Character character);
		void updateStats(GW2LIB::Character &character);
		void reset();

		string getName() { return name; }
		int getDodgeCount() { return dodgeCount; }
		float getTotalDamageTaken() { return totalDamageTaken; }
};
