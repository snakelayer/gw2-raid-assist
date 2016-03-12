#pragma once

#include <sstream>
#include <vector>

#include <boost/assign.hpp>
#include <boost/format.hpp>

#include "gw2lib.h"

#include "../../../assist_drawer.h"

class ImbuedMushroom
{
    public:
        ImbuedMushroom();

        void updateState(int elapsedMilliseconds);

        void draw();

    private:
        static const int SPAWN_INTERVAL_MS = 50 * 1000;
        // these agents have sequence = 463
        static std::vector<GW2LIB::Vector3> POSITIONS;
        int elapsedMilliseconds;

        int getNextSpawnTime();
        int getNextMushroomIndex();
};