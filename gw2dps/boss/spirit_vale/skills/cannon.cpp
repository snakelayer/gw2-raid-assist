#include "cannon.h"

using namespace boost;
using namespace boost::assign;
using namespace GW2LIB;
using namespace std;

const float Cannon::MAX_HP = 74700.0f;

map<COMPASS::DIRECTION, Vector3> Cannon::launchPositionMap = map_list_of
(COMPASS::DIRECTION::NORTH, Vector3(-4980, 4390, -2480))
(COMPASS::DIRECTION::EAST, Vector3(-4020, 3190, -2480))
(COMPASS::DIRECTION::SOUTH, Vector3(-5220, 2180, -2480))
(COMPASS::DIRECTION::WEST, Vector3(-6200, 3400, -2450));

map<COMPASS::DIRECTION, Vector3> Cannon::spawnLocationMap = map_list_of
(COMPASS::DIRECTION::NORTH, Vector3(-4924.790039f, 6268.450195f, -3044.570068f))
(COMPASS::DIRECTION::EAST, Vector3(-2080.840088f, 3099.270020f, -3050.340088f))
(COMPASS::DIRECTION::SOUTH, Vector3(-5286.229980f, 343.825989f, -3036.719971f))
(COMPASS::DIRECTION::WEST, Vector3(-8125.769531f, 3553.649902f, -3056.870117f));

map<COMPASS::DIRECTION, string> Cannon::directionNameMap = map_list_of
(COMPASS::DIRECTION::NORTH, "N")
(COMPASS::DIRECTION::EAST, "E")
(COMPASS::DIRECTION::SOUTH, "S")
(COMPASS::DIRECTION::WEST, "W");

map<int, COMPASS::DIRECTION> Cannon::rotationMap = map_list_of
(0, COMPASS::DIRECTION::SOUTH)
(1, COMPASS::DIRECTION::WEST)
(2, COMPASS::DIRECTION::NORTH)
(3, COMPASS::DIRECTION::EAST)
(4, COMPASS::DIRECTION::SOUTH)
(5, COMPASS::DIRECTION::NORTH)
(6, COMPASS::DIRECTION::WEST)
(7, COMPASS::DIRECTION::EAST);

Cannon::Cannon() : elapsedSeconds(0), meter(80.0f, 8.0f) {
}

void Cannon::updateState(int elapsedSeconds) {
    this->elapsedSeconds = elapsedSeconds;
    findActiveCannons();
}

void Cannon::draw() {
    drawCompass();
    drawLineToNextCannon();
}

bool Cannon::isAliveCannon(Agent agent) {
    if (agent.IsValid() && (agent.GetType() == GW2::AgentType::AGENT_TYPE_GADGET_ATTACK_TARGET) && (agent.IsSelectable())) {
        AttackTarget at = agent.GetAttackTarget();
        return at.IsValid() && (at.GetMaxHealth() == MAX_HP) && (at.GetCurrentHealth() != 0.0f);
    }

    return false;
}

void Cannon::findActiveCannons() {
    Agent agent;

    activeCannons.clear();
    while (agent.BeNext()) {
        if (isAliveCannon(agent)) {
            activeCannons.insert(pair<COMPASS::DIRECTION, GW2LIB::Agent>(getCannonLocation(agent), agent));
        }
    }
}

void Cannon::drawCompass() {
    Vector3 selfPos = GetOwnAgent().GetPos();
    DrawCircleProjected(selfPos, COMPASS_RADIUS, AssistDrawer::WHITE);

    stringstream ss;
    for (int rawDirection = COMPASS::DIRECTION::NORTH; rawDirection <= COMPASS::DIRECTION::WEST; ++rawDirection) {
        COMPASS::DIRECTION direction = static_cast<COMPASS::DIRECTION>(rawDirection);

        float x, y;
        WorldToScreen(getDirectionPosition(selfPos, direction), &x, &y);

        map<COMPASS::DIRECTION, Agent>::iterator it = activeCannons.find(direction);
        if (it != activeCannons.end()) {
            Agent agent = it->second;
            float percent = agent.GetAttackTarget().GetCurrentHealth() / MAX_HP;
            meter.drawAtPercent(x, y, AssistDrawer::RED, percent);
            continue;
        }

        ss.str("");
        if (direction == getNextSpawnDirection()) {
            ss << format("%s: %d") % directionNameMap[direction] % (COOLDOWN - (elapsedSeconds % COOLDOWN));
        }
        else {
            ss << format("%s") % directionNameMap[direction];
        }

        AssistDrawer::get().drawStreamToWindow(ss, x, y);
    }
}

void Cannon::drawLineToNextCannon() {
    COMPASS::DIRECTION direction = getNextSpawnDirection();
    Vector3 launchPos = getCannonLaunchPosition(direction);
    Vector3 selfPos = GetOwnAgent().GetPos();
    Vector3 partialLaunchPoint(
        selfPos.x + ((launchPos.x - selfPos.x) * CANNON_LINE_FRACTION),
        selfPos.y + ((launchPos.y - selfPos.y) * CANNON_LINE_FRACTION),
        selfPos.z + ((launchPos.z - selfPos.z) * CANNON_LINE_FRACTION));

    DrawLineProjected(selfPos, partialLaunchPoint, AssistDrawer::WHITE);
}

COMPASS::DIRECTION Cannon::getNextSpawnDirection() {
    int rotations = (elapsedSeconds / 30) % 8;
    return rotationMap[rotations];
}

Vector3 Cannon::getDirectionPosition(Vector3 origin, COMPASS::DIRECTION direction) {
    if (direction == COMPASS::DIRECTION::NORTH) {
        return Vector3(
            origin.x,
            origin.y + COMPASS_DIRECTION_LENGTH,
            origin.z - AssistDrawer::lineHeight
            );
    }
    else if (direction == COMPASS::DIRECTION::EAST) {
        return Vector3(
            origin.x + COMPASS_DIRECTION_LENGTH,
            origin.y,
            origin.z - AssistDrawer::lineHeight
            );
    }
    else if (direction == COMPASS::DIRECTION::SOUTH) {
        return Vector3(
            origin.x,
            origin.y - COMPASS_DIRECTION_LENGTH,
            origin.z - AssistDrawer::lineHeight
            );
    }
    else if (direction == COMPASS::DIRECTION::WEST) {
        return Vector3(
            origin.x - COMPASS_DIRECTION_LENGTH,
            origin.y,
            origin.z - AssistDrawer::lineHeight
            );
    }
    else { // shut up compiler
        return origin;
    }
}

Vector3 Cannon::getCannonLaunchPosition(COMPASS::DIRECTION direction) {
    return launchPositionMap[direction];
}

COMPASS::DIRECTION Cannon::getCannonLocation(Agent agent) {
    Vector3 pos = agent.GetPos();

    for (map<COMPASS::DIRECTION, Vector3>::iterator it = spawnLocationMap.begin(); it != spawnLocationMap.end(); ++it) {
        Vector3 spawnPos = it->second;
        if (spawnPos.x == pos.x && spawnPos.y == pos.y && spawnPos.z == pos.z) {
            return it->first;
        }
    }

    return COMPASS::DIRECTION::UNKNOWN;
}
