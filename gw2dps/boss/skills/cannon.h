#pragma once

#include <map>

#include <boost/assign.hpp>
#include <boost/format.hpp>

#include "gw2lib.h"

#include "../../assist_drawer.h"

namespace COMPASS {
	enum DIRECTION {
		NORTH = 0,
		EAST,
		SOUTH,
		WEST
	};
}

class Cannon
{
	public:
		void draw(int elapsedSeconds);

	private:
		static const int COOLDOWN = 30;

		static std::map<COMPASS::DIRECTION, GW2LIB::Vector3> launchPositionMap;

		const float COMPASS_RADIUS = 240.0f;
		const float COMPASS_DIRECTION_LENGTH = 250.0f;
		const float CANNON_LINE_FRACTION = 0.4f;
		static std::map<int, COMPASS::DIRECTION> rotationMap;

		void drawCompass(COMPASS::DIRECTION direction, int remainingCooldown);
		void drawLineToNextCannon(COMPASS::DIRECTION direction);

		COMPASS::DIRECTION getNextCannonDirection(int elapsedSeconds);
		GW2LIB::Vector3 getDirectionPosition(GW2LIB::Vector3 origin, COMPASS::DIRECTION direction);
		GW2LIB::Vector3 getCannonLaunchPosition(COMPASS::DIRECTION direction);
};