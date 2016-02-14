#pragma once

#include <boost/format.hpp>
#include <boost/timer/timer.hpp>

#include "gw2lib.h"

#include "../../assist_drawer.h"

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
		float getCooldown();
		void startFirstCharge();
		bool tryUpdateRotation(float rotation);
		void tryStartAttack();
		void disable();
		void startActivating();

		void drawCooldownMeter(float x, float y);
		void drawActivatingMarker(GW2LIB::Vector3 position);
		void drawActiveMarker(GW2LIB::Vector3 position);

	private:
		const float FIRST_COOLDOWN = 30.0f; // sometimes delayed to 31
		const float COOLDOWN = 30.0f; // sometimes delayed to 33
		const float ACTIVATING_DURATION = 2.5f;
		const float ACTIVE_DURATION = 10.0f;

		const float meterWidth = 150.0f;
		const float meterHeight = 18.0f;
		const float flamewallWidth = 30.0f;
		const float flamewallLength = 500.0f;

		const DWORD ACTIVATING_BASE = 0x40ffffff; // white
		const DWORD ACTIVATING_FILL = 0x80f02020; // red-ish
		const DWORD ACTIVE = 0x806060f0; // light blue

		FW::State state;
		boost::timer::cpu_timer timer;
		float startingRotation;

		void startRecharge();

		bool isFinishedFirstCharging();
		bool isFinishedRecharging();
		bool isFinishedActivating();
		bool isFinishedAttacking();

		float getElapsedSeconds();

		void drawFlamewallRect(GW2LIB::Vector3 position, float length, float width, float rotation, DWORD color);
};