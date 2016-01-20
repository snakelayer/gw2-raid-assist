#pragma once

#include <boost/timer/timer.hpp>

namespace MS {
	enum State
	{
		PENDING = 0,
		READY,
		ACTIVE,
		RECHARGING
	};
}

class MagicStorm
{
	public:
		MagicStorm();
		MS::State getState();
		void setState(MS::State state);
		int getCooldown();

	private:
		const float COOLDOWN = 30.0f;

		MS::State state;
		boost::timer::cpu_timer timer;
};