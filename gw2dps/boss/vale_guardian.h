#pragma once

#include <sstream>

#include <boost/circular_buffer.hpp>
#include <boost/format.hpp>
#include <boost/timer/timer.hpp>

#include "gw2lib.h"

#include "raid_boss.h"

using namespace boost;
using namespace std;
using namespace GW2LIB;

namespace VG {

	enum Phase {
		FIRST = 0,
		SECOND,
		THIRD,
		SPLIT
	};

	enum MagicStormState {
		PENDING = 0,
		READY,
		ACTIVE,
		RECHARGING
	};

	struct MagicStorm {
		MagicStormState state;
		double cooldown;
	};
}

class ValeGuardian : public RaidBoss
{
	private:
		static const float MAX_HP;
		const float FIRST_PHASE_TRANSITION_HP = 14384054;
		const float SECOND_PHASE_TRANSITION_HP = MAX_HP / 3; //TODO: verify
		const float MAGIC_STORM_COOLDOWN = 30.0f;

		boost::timer::cpu_timer cooldownTimer;
		VG::MagicStorm magicStorm;
		VG::Phase phase;

		ValeGuardian(Agent agent);

		void addEstTimeToSplit(stringstream &ss);
		void addMagicStormStatus(stringstream &ss);

		double getSecondsUntilMagicStormReady();
		bool refreshAgent();

	public:
		~ValeGuardian();
		static RaidBoss* instance(Agent agent) { return new ValeGuardian(agent); }

		string getName() { return "Vale Guardian"; }
		static bool matchesTarget(Agent &agent);

		void updateState(boost::circular_buffer<float> &damageBuffer);
		void updateSquadState(Squad *squad);
		void outputAssistInfo(stringstream &ss);

		void outputDebug(stringstream &ss);
};
