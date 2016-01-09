#pragma once

#include "gw2lib.h"

#include "raid_boss.h"

using namespace GW2LIB;

class Gorseval : public RaidBoss
{
	private:
		static const float MAX_HP;

		Gorseval(Agent &agent);

	public:
		static RaidBoss* instance(Agent &agent) { return new Gorseval(agent); }

		string getName() { return "Gorseval"; }
		static bool matchesTarget(Agent &agent);

		void updateState(boost::circular_buffer<float> &damageBuffer);
		void outputAssistInfo(stringstream &ss);

		void outputDebug(stringstream &ss) {}
};