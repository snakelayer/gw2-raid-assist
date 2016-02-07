#pragma once

#include "gw2lib.h"

#include <boost/assign.hpp>
#include <boost/timer/timer.hpp>

class Seeker
{
	public:
		static float getMaxHp() { return MAX_HP; }
		static bool isSpawnPosition(GW2LIB::Agent agent);

		Seeker(GW2LIB::Agent agent);
		int getRespawnTime();
		GW2LIB::Vector3 getPosition();

	private:
		static const float MAX_HP;
		static const std::list<GW2LIB::Vector3> spawnPositions;
		static const float SPAWN_LOCATION_TOLERANCE; //TODO: do we really need this?

		const float COOLDOWN = 21.0f;

		GW2LIB::Agent agent;
		boost::timer::cpu_timer timer;
};