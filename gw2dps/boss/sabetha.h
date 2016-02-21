#pragma once

#include <map>

#include <boost/assign.hpp>
#include <boost/circular_buffer.hpp>

#include "gw2lib.h"

#include "raid_boss.h"
#include "skills/flamewall.h"

#include "../assist_drawer.h"

namespace SABETHA {
	enum Phase {
		FIRST = 0,
		KERNAN,
		SECOND,
		KNUCKLES,
		THIRD,
		KARDE,
		FOURTH
	};

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

		const float KERNAN_MAX_HP = 3440850;
		const float KNUCKLES_MAX_HP = 3440850;
		const float KARDE_MAX_HP = 4423950;

		/*
		  this is intended only to trigger on:
		   - cannon barrage
		   - hail of bullets (3rd volley),
		  both of which do about 6k damage
		 */
		static const float HEAVY_HIT_DAMAGE_THRESHOLD;

		const float flamewallDisplayOffset = 20.0f;

		SABETHA::Phase phase;
		Flamewall flamewall;
		float startingRotation;

		const GW2LIB::Vector3 SABETHA_STARTING_POSITION = GW2LIB::Vector3(
			-5078.209961f,
			3356.969971f,
			-2457.178711f);
		const GW2LIB::Vector3 SABETHA_PHASE_POSITION;

		static std::map<SABETHA::COMPASS, GW2LIB::Vector3> launchPositionMap;

		const float COMPASS_RADIUS = 240.0f;
		const float COMPASS_DIRECTION_LENGTH = 250.0f;
		const float CANNON_LINE_FRACTION = 0.4f;
		static std::map<int, SABETHA::COMPASS> cannonRotationMap;

		Sabetha(GW2LIB::Agent agent);

		float getMaxHp() { return MAX_HP; }
		float getBossHeight() { return 300.0f; } //TODO: check this
		bool isAtStartPosition() {
			return (SABETHA_STARTING_POSITION.x == agent.GetPos().x) &&
				   (SABETHA_STARTING_POSITION.y == agent.GetPos().y) &&
				   (SABETHA_STARTING_POSITION.z == agent.GetPos().z);
		}

		void updateFlamewallState();
		void drawFlamewallStatus();

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
		float getHeavyHitDamageThreshold() { return HEAVY_HIT_DAMAGE_THRESHOLD; }
		void drawAssistInfo();

		void outputDebug(std::stringstream &ss);
};