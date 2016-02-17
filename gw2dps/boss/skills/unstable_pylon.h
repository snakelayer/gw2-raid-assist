#pragma once

#include <boost/timer/timer.hpp>

namespace UP {
	enum State
	{
		PENDING_SPLIT = 0,
		PENDING_MOVE_FIRST,
		SAFE_RED_BLUE,
		SAFE_GREEN_RED,
		SAFE_BLUE_GREEN,
		PENDING_MOVE_SECOND,
		SAFE_RED,
		SAFE_GREEN,
		SAFE_BLUE
	};
}

class UnstablePylon
{
	public:
		UnstablePylon();

		void startWith(UP::State state);
		void update();
		void stop();

	private:
		const float FIRST_COOLDOWN = 20.0f;
		const float SECOND_COOLDOWN = 17.0f;
		const float MOVE_COOLDOWN = 4.0f;

		UP::State state;
		boost::timer::cpu_timer timer;

		bool isFirst();
		bool isSecond();

		void tick();
		float getFirstCooldown();
		float getSecondCooldown();
		float getMoveCooldown();
};
