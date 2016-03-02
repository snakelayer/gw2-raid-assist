#include "cannon.h"

using namespace boost;
using namespace boost::assign;
using namespace GW2LIB;
using namespace std;

map<COMPASS::DIRECTION, Vector3> Cannon::launchPositionMap = map_list_of
(COMPASS::DIRECTION::NORTH, Vector3(-4980, 4390, -2480))
(COMPASS::DIRECTION::EAST, Vector3(-4020, 3190, -2480))
(COMPASS::DIRECTION::SOUTH, Vector3(-5220, 2180, -2480))
(COMPASS::DIRECTION::WEST, Vector3(-6200, 3400, -2450));

map<int, COMPASS::DIRECTION> Cannon::rotationMap = map_list_of
(0, COMPASS::DIRECTION::SOUTH)
(1, COMPASS::DIRECTION::WEST)
(2, COMPASS::DIRECTION::NORTH)
(3, COMPASS::DIRECTION::EAST)
(4, COMPASS::DIRECTION::SOUTH)
(5, COMPASS::DIRECTION::NORTH)
(6, COMPASS::DIRECTION::WEST)
(7, COMPASS::DIRECTION::EAST);

void Cannon::draw(int elapsedSeconds) {
    COMPASS::DIRECTION direction = getNextCannonDirection(elapsedSeconds);
    int remainingCooldown = COOLDOWN - (elapsedSeconds % COOLDOWN);

    drawCompass(direction, remainingCooldown);
    drawLineToNextCannon(direction);
}

void Cannon::drawCompass(COMPASS::DIRECTION direction, int remainingCooldown) {
    Vector3 selfPos = GetOwnAgent().GetPos();
    DrawCircleProjected(selfPos, COMPASS_RADIUS, AssistDrawer::WHITE);

    float x, y;

    WorldToScreen(getDirectionPosition(selfPos, COMPASS::DIRECTION::NORTH), &x, &y);
    AssistDrawer::get().drawFont(x, y, AssistDrawer::WHITE, (direction == COMPASS::DIRECTION::NORTH) ? str(format("N: %d") % remainingCooldown) : "N");
    WorldToScreen(getDirectionPosition(selfPos, COMPASS::DIRECTION::EAST), &x, &y);
    AssistDrawer::get().drawFont(x, y, AssistDrawer::WHITE, (direction == COMPASS::DIRECTION::EAST) ? str(format("E: %d") % remainingCooldown) : "E");
    WorldToScreen(getDirectionPosition(selfPos, COMPASS::DIRECTION::SOUTH), &x, &y);
    AssistDrawer::get().drawFont(x, y, AssistDrawer::WHITE, (direction == COMPASS::DIRECTION::SOUTH) ? str(format("S: %d") % remainingCooldown) : "S");
    WorldToScreen(getDirectionPosition(selfPos, COMPASS::DIRECTION::WEST), &x, &y);
    AssistDrawer::get().drawFont(x, y, AssistDrawer::WHITE, (direction == COMPASS::DIRECTION::WEST) ? str(format("W: %d") % remainingCooldown) : "W");
}

void Cannon::drawLineToNextCannon(COMPASS::DIRECTION direction) {
    Vector3 selfPos = GetOwnAgent().GetPos();
    Vector3 launchPos = getCannonLaunchPosition(direction);
    Vector3 partialLaunchPoint(
        selfPos.x + ((launchPos.x - selfPos.x) * CANNON_LINE_FRACTION),
        selfPos.y + ((launchPos.y - selfPos.y) * CANNON_LINE_FRACTION),
        selfPos.z + ((launchPos.z - selfPos.z) * CANNON_LINE_FRACTION));

    DrawLineProjected(selfPos, partialLaunchPoint, AssistDrawer::WHITE);
}

COMPASS::DIRECTION Cannon::getNextCannonDirection(int elapsedSeconds) {
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
