#pragma once

#include <boost/circular_buffer.hpp>

#include "raid_boss.h"

class Sabetha : public RaidBoss
{
	private:
		static const float MAX_HP;
		const float FIRST_PHASE_TRANSITION_HP = 25497542;
		const float SECOND_PHASE_TRANSITION_HP = 16643379;
		const float THIRD_PHASE_TRANSITION_HP = MAX_HP/4; // TODO verify

		Sabetha(Agent agent);

	public:

		static RaidBoss* instance(Agent agent) { return new Sabetha(agent); }
		static bool matchesTarget(Agent &agent);

		string getName() { return "Sabetha"; }

		void updateState(boost::circular_buffer<float> &damageBuffer);
		void updateSquadState(SquadMemberMap &members);
		void outputAssistInfo(stringstream &ss);

		void outputDebug(stringstream &ss);
};