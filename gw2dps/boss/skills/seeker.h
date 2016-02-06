#pragma once

#include "gw2lib.h"

#include <boost/timer/timer.hpp>

class Seeker
{
	public:
		static float getMaxHp() { return MAX_HP; }

		Seeker(GW2LIB::Agent agent);
		int getRespawnTime();
		GW2LIB::Vector3 getPosition();

	private:
		static const float MAX_HP;
		const float COOLDOWN = 21.0f;

		GW2LIB::Agent agent;
		GW2LIB::Vector3 position;
		boost::timer::cpu_timer timer;
};