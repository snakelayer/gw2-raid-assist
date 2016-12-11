#pragma once

#include <map>
#include <sstream>

#include <boost/assign.hpp>
#include <boost/format.hpp>

#include "gw2lib.h"

#include "../../../draw/meter.h"
#include "../../../assist_drawer.h"

namespace COMPASS {
    enum DIRECTION {
        UNKNOWN = 0,
        NORTH,
        EAST,
        SOUTH,
        WEST
    };
}

class Cannon
{
    public:
        Cannon();

        void updateState(int elapsedSeconds);

        void draw();

    private:
        static const float MAX_HP;
        static const int COOLDOWN = 30;

        static std::map<COMPASS::DIRECTION, GW2LIB::Vector3> launchPositionMap;
        static std::map<COMPASS::DIRECTION, GW2LIB::Vector3> spawnLocationMap;
        static std::map<COMPASS::DIRECTION, std::string> directionNameMap;

        const float COMPASS_RADIUS = 240.0f;
        const float COMPASS_DIRECTION_LENGTH = 250.0f;
        const float CANNON_LINE_FRACTION = 0.4f;
        static std::map<int, COMPASS::DIRECTION> rotationMap;

        int elapsedSeconds;
        std::map<COMPASS::DIRECTION, GW2LIB::Agent> activeCannons;

        Meter meter;

        bool isAliveCannon(GW2LIB::Agent agent);

        void findActiveCannons();

        void drawCompass();
        void drawLineToNextCannon();

        COMPASS::DIRECTION getNextSpawnDirection();
        GW2LIB::Vector3 getDirectionPosition(GW2LIB::Vector3 origin, COMPASS::DIRECTION direction);
        GW2LIB::Vector3 getCannonLaunchPosition(COMPASS::DIRECTION direction);
        COMPASS::DIRECTION getCannonLocation(GW2LIB::Agent agent);
};
