#pragma once

#include <boost/format.hpp>
#include <boost/timer/timer.hpp>

#include "gw2lib.h"

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
		void drawStatusMeter(float x, float y, float breakbar);

	private:
		const float COOLDOWN = 30.0f;

		MS::State state;
		boost::timer::cpu_timer timer;

		const float meterWidth = 150.0f;
		const float meterHeight = 18.0f;

		float getCooldown();
};