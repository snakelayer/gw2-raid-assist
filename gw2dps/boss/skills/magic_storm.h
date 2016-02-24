#pragma once

#include <boost/format.hpp>
#include <boost/timer/timer.hpp>

#include "gw2lib.h"

#include "draw/meter.h"
#include "../../assist_drawer.h"

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
		void drawStatusMeter(float x, float y, float percent);

	private:
		const float COOLDOWN = 30.0f;

		MS::State state;
		boost::timer::cpu_timer timer;

		Meter meter;

		float getCooldown();
};