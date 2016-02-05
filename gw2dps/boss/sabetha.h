#pragma once

#include <map>

#include <boost/assign.hpp>
#include <boost/circular_buffer.hpp>

#include "gw2lib.h"

#include "raid_boss.h"

namespace SABETHA {
	enum COMPASS {
		NORTH = 0,
		EAST,
		SOUTH,
		WEST
	};
}

class Sabetha : public RaidBoss
{
	private:
		static const float MAX_HP;
		const float FIRST_PHASE_TRANSITION_HP = 25497542;
		const float SECOND_PHASE_TRANSITION_HP = 16643379;
		const float THIRD_PHASE_TRANSITION_HP = MAX_HP/4; // TODO verify

		static std::map<SABETHA::COMPASS, GW2LIB::Vector3> launchPositionMap;

		const float COMPASS_RADIUS = 50.0f;
		const float COMPASS_DIRECTION_LENGTH = 60.0f;
		const float CANNON_LINE_FRACTION = 0.4f;
		static std::map<int, SABETHA::COMPASS> cannonRotationMap;

		GW2LIB::Font font;
		static const int fontSize = 14;
		static const DWORD BLACK = 0x00000000;
		static const DWORD WHITE = 0xFFFFFFFF;
		static const DWORD GREEN = 0x00FF00FF; // ??

		Sabetha(GW2LIB::Agent agent);

		float getMaxHp() { return MAX_HP; }

		SABETHA::COMPASS getNextCannonDirection();
		GW2LIB::Vector3 getDirectionPosition(GW2LIB::Vector3 origin, SABETHA::COMPASS direction);
		void drawCompass();
		void drawLineToNextCannon();
		GW2LIB::Vector3 getCannonLaunchPosition(SABETHA::COMPASS direction);

	public:

		static RaidBoss* instance(GW2LIB::Agent agent) { return new Sabetha(agent); }
		static bool matchesTarget(GW2LIB::Agent &agent);

		std::string getName() { return "Sabetha"; }

		void updateState(boost::circular_buffer<float> &damageBuffer);
		void updateSquadState(SquadMemberMap &members);
		void outputAssistInfo(std::stringstream &ss);

		void outputDebug(std::stringstream &ss);
};