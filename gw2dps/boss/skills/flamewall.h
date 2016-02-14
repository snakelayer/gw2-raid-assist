#pragma once

#include <boost/timer/timer.hpp>

namespace FW {
	enum State
	{
		PENDING_START = 0, // before raid begins
		PENDING_CENTER,    // when Sabetha is hiding
		FIRST_CHARGE,      // the first 30-second cooldown
		RECHARGING,        // in subsequent cooldowns
		ACTIVATING,        // marker appearing until flamewall hitbox activating
		ACTIVE,            // flamewall hitbox is active
	};
}

class Flamewall
{
	public:
		Flamewall();
		FW::State getState();
		void startFirstCharge();
		bool tryUpdateRotation(float rotation);
		void tryStartAttack();
		void disable();
		void startActivating();

	private:
		const float FIRST_COOLDOWN = 30.0f;
		const float COOLDOWN = 30.0f;
		const float ACTIVATING_DURATION = 3.5f;
		const float ACTIVE_DURATION = 10.0f;

		FW::State state;
		boost::timer::cpu_timer timer;
		float startingRotation;

		void startRecharge();

		bool isFinishedFirstCharging();
		bool isFinishedRecharging();
		bool isFinishedActivating();
		bool isFinishedAttacking();

		float getElapsedSeconds();
};