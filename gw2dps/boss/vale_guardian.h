#pragma once

#include <sstream>

#include <boost/circular_buffer.hpp>
#include <boost/format.hpp>
#include <boost/timer/timer.hpp>

#include "gw2lib.h"

#include "raid_boss.h"
#include "skills/magic_storm.h"
#include "skills/seeker.h"

using namespace boost;
using namespace std;
using namespace GW2LIB;

namespace VG {

	enum Phase {
		FIRST = 0,
		FIRST_TRANSITION,
		FIRST_SPLIT,
		SECOND,
		SECOND_TRANSITION,
		SECOND_SPLIT,
		THIRD
	};
}

class ValeGuardian : public RaidBoss
{
	private:
		static const float MAX_HP;
		const float FIRST_PHASE_TRANSITION_HP = 14384054; // also possible 14,408,081 14,390,254
		const float SECOND_PHASE_TRANSITION_HP = 7244926;
		const float MAGIC_STORM_COOLDOWN = 30.0f;

		// for reference, HP of each guardian at the entrance:
		// red:   2457750
		// green: 3440850
		// blue:  3539160
		const float RED_GUARDIAN_MAX_HP = 540705;
		const float GREEN_GUARDIAN_MAX_HP = 722579;
		const float BLUE_GUARDIAN_MAX_HP = 540705; // TODO: verify

		Seeker seeker;
		MagicStorm magicStorm;
		VG::Phase phase;

		ValeGuardian(Agent agent);

		void addEstTimeToSplit(stringstream &ss);
		void addMagicStormStatus(stringstream &ss);
		void addSeekerStatus(stringstream &ss);

		bool reacquireValeGuardian();
		bool findRedGuardian();

	public:
		~ValeGuardian();
		static RaidBoss* instance(Agent agent) { return new ValeGuardian(agent); }

		string getName() { return "Vale Guardian"; }
		static bool matchesTarget(Agent &agent);

		void updateState(boost::circular_buffer<float> &damageBuffer);
		void updateSquadState(SquadMemberMap &members);
		void outputAssistInfo(stringstream &ss);

		void outputDebug(stringstream &ss);
};
