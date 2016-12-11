#include "seeker.h"

using namespace boost::assign;
using namespace GW2LIB;

const float Seeker::MAX_HP = 649260;
const std::list<Vector3> Seeker::spawnPositions = list_of
    (Vector3(-5537, -20191, -2406)) // green
    (Vector3(-3992, -20173, -2406)) // blue
    (Vector3(-4770, -21516, -2407)) // red
    (Vector3(-4749, -20607, -2401)); // center
const float Seeker::SPAWN_LOCATION_TOLERANCE = 50.0f;

bool Seeker::isSpawnPosition(Agent agent) {
    for (Vector3 spawnPosistion : spawnPositions) {
        if ((abs(spawnPosistion.x - agent.GetPos().x) < SPAWN_LOCATION_TOLERANCE) &&
            (abs(spawnPosistion.y - agent.GetPos().y) < SPAWN_LOCATION_TOLERANCE) &&
            (abs(spawnPosistion.z - agent.GetPos().z) < SPAWN_LOCATION_TOLERANCE)) {
            return true;
        }
    }

    return false;
}

Seeker::Seeker(Agent agent) : agent(agent) {
}

int Seeker::getRespawnTime() {
    double secondsElapsed = timer.elapsed().wall / 1e9;
    return (int)(COOLDOWN - secondsElapsed);
}

Vector3 Seeker::getPosition() {
    return agent.GetPos();
}

