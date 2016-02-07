#pragma once

#include <sstream>

#include <boost/circular_buffer.hpp>
#include <boost/format.hpp>
#include <boost/timer/timer.hpp>

#include "gw2lib.h"

#include "raid_boss.h"
#include "skills/magic_storm.h"
#include "skills/seeker.h"

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
		const float FIRST_PHASE_TRANSITION_HP = 14553669;
		const float SECOND_PHASE_TRANSITION_HP = 7255058;
		const float MAGIC_STORM_COOLDOWN = 30.0f;

		// for reference, HP of each guardian at the entrance:
		// red:   2457750
		// green: 3440850
		// blue:  3539160
		const float RED_GUARDIAN_MAX_HP = 540705;
		const float GREEN_GUARDIAN_MAX_HP = 722579;
		const float BLUE_GUARDIAN_MAX_HP = 540705; // TODO: verify

		/*
		  based on 1000 toughness:

		  magic aura: <1.8k
		  (blue) unstable magic spike: 4K+
		  (green) distributed magic (<4 players): 13+K
		  red orb:
		  unstable pylon: 2k
		  magic storm:
		  punch: 4k+
		  bullet storm: 3k+
		*/
		static const int HEAVY_HIT_DAMAGE_THRESHOLD = -1800;

		std::list<Seeker> seekers;
		MagicStorm magicStorm;
		VG::Phase phase;

		ValeGuardian(GW2LIB::Agent agent);

		float getMaxHp() { return MAX_HP; }
		float getBossHeight() { return 350.0f; }
		bool isSplit() { return !(phase == VG::Phase::FIRST || phase == VG::Phase::SECOND || phase == VG::Phase::THIRD); }

		void updateSeekerState();

		void addEstTimeToSplit(std::stringstream &ss);
		void addMagicStormStatus(std::stringstream &ss);
		void drawSeekerStatus();

		bool findRedGuardian();

	public:
		~ValeGuardian();

		static RaidBoss* instance(GW2LIB::Agent agent) { return new ValeGuardian(agent); }
		static bool matchesTarget(GW2LIB::Agent &agent);

		std::string getName() { return "Vale Guardian"; }

		void updateState(boost::circular_buffer<float> &damageBuffer);
		void updateSquadState(SquadMemberMap &members);
		void drawAssistInfo();


		bool isDead() { return !isSplit() && RaidBoss::isDead(); }

		void outputDebug(std::stringstream &ss);
};
