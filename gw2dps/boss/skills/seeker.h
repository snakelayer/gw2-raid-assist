#pragma once

#include "gw2lib.h"

#include <boost/timer/timer.hpp>

namespace SEEKER {
	enum State
	{
		INACTIVE = 0,
		ACTIVE
	};
}

class Seeker
{
	public:
		Seeker();
		SEEKER::State getState();
		int getRespawnTime();

	private:
		const float MAX_HP = 649260;
		const float COOLDOWN = 20.0f;

		SEEKER::State state;
		boost::timer::cpu_timer timer;

		bool isSpawned();
};