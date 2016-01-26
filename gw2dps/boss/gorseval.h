#pragma once

#include "gw2lib.h"

#include "raid_boss.h"

using namespace GW2LIB;

class Gorseval : public RaidBoss
{
	private:
		static const float MAX_HP;
		const float FIRST_TRANSITION_HP = 14190625;
		const float SECOND_TRANSITION_HP = 7020792;

		/*
		  Haunting Aura is generally around 1.2k
		  at 1000 toughness:
		   swipe is 3.1k - 3.3k
		   spectral impact is 4.1k - 5.7k
		  at 1600 toughness:
		   swipe is 2.4k - 2.6k
		   spectral impact is 3.0k - 4.5k
		*/

		// anything below this is considered a dodgeable hit the player took
		static const int PLAYER_HIT_DAMAGE_THRESHOLD = -1500;

		Gorseval(Agent agent);

	public:
		static RaidBoss* instance(Agent agent) { return new Gorseval(agent); }

		string getName() { return "Gorseval"; }
		static bool matchesTarget(Agent &agent);

		void updateState(boost::circular_buffer<float> &damageBuffer);
		void updateSquadState(Squad *squad);
		void outputAssistInfo(stringstream &ss);

		void outputDebug(stringstream &ss) {}
};