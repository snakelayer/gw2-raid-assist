#include "sabetha.h"

using namespace boost;
using namespace GW2LIB;
using namespace std;

const float Sabetha::MAX_HP = 34015256;
map<SABETHA::COMPASS, Vector3> Sabetha::launchPositionMap = boost::assign::map_list_of
	(SABETHA::COMPASS::NORTH, Vector3(-4980, 4390, -2480))
	(SABETHA::COMPASS::EAST,  Vector3(-4020, 3190, -2480))
	(SABETHA::COMPASS::SOUTH, Vector3(-5220, 2180, -2480))
	(SABETHA::COMPASS::WEST,  Vector3(-6200, 3400, -2450));

map<int, SABETHA::COMPASS> Sabetha::cannonRotationMap = boost::assign::map_list_of
	(0, SABETHA::COMPASS::SOUTH)
	(1, SABETHA::COMPASS::WEST)
	(2, SABETHA::COMPASS::NORTH)
	(3, SABETHA::COMPASS::EAST)
	(4, SABETHA::COMPASS::SOUTH)
	(5, SABETHA::COMPASS::NORTH)
	(6, SABETHA::COMPASS::WEST)
	(7, SABETHA::COMPASS::EAST);

Sabetha::Sabetha(Agent agent) : RaidBoss(agent)
{
	font.Init(fontSize, "Verdana");
}

SABETHA::COMPASS Sabetha::getNextCannonDirection() {
	int rotations = (encounterTimer.getElapsedSeconds() / 30) % 8;
	return cannonRotationMap[rotations];
}

Vector3 Sabetha::getDirectionPosition(Vector3 origin, SABETHA::COMPASS direction) {
	if (direction == SABETHA::COMPASS::NORTH) {
		return Vector3(
			origin.x,
			origin.y + COMPASS_DIRECTION_LENGTH,
			origin.z - fontSize
		);
	}
	else if (direction == SABETHA::COMPASS::EAST) {
		return Vector3(
			origin.x + COMPASS_DIRECTION_LENGTH,
			origin.y,
			origin.z - fontSize
		);
	}
	else if (direction == SABETHA::COMPASS::SOUTH) {
		return Vector3(
			origin.x,
			origin.y - COMPASS_DIRECTION_LENGTH,
			origin.z - fontSize
		);
	}
	else if (direction == SABETHA::COMPASS::WEST) {
		return Vector3(
			origin.x - COMPASS_DIRECTION_LENGTH,
			origin.y,
			origin.z - fontSize
		);
	}
	else { // shut up compiler
		return origin;
	}
}

void Sabetha::drawCompass() {
	Vector3 selfPos = GetOwnAgent().GetPos();
	DrawCircleProjected(selfPos, COMPASS_RADIUS, WHITE);
	SABETHA::COMPASS direction = getNextCannonDirection();
	int secondsUntilNextCannon = 30 - (encounterTimer.getElapsedSeconds() % 30);

	float x, y;

	WorldToScreen(getDirectionPosition(selfPos, SABETHA::COMPASS::NORTH), &x, &y);
	font.Draw(x, y, WHITE, (direction == SABETHA::COMPASS::NORTH) ? str(format("N: %d") % secondsUntilNextCannon) : "N");
	WorldToScreen(getDirectionPosition(selfPos, SABETHA::COMPASS::EAST), &x, &y);
	font.Draw(x, y, WHITE, (direction == SABETHA::COMPASS::EAST) ? str(format("E: %d") % secondsUntilNextCannon) : "E");
	WorldToScreen(getDirectionPosition(selfPos, SABETHA::COMPASS::SOUTH), &x, &y);
	font.Draw(x, y, WHITE, (direction == SABETHA::COMPASS::SOUTH) ? str(format("S: %d") % secondsUntilNextCannon) : "S");
	WorldToScreen(getDirectionPosition(selfPos, SABETHA::COMPASS::WEST), &x, &y);
	font.Draw(x, y, WHITE, (direction == SABETHA::COMPASS::WEST) ? str(format("W: %d") % secondsUntilNextCannon) : "W");
}

void Sabetha::drawLineToNextCannon() {
	Vector3 selfPos = GetOwnAgent().GetPos();
	SABETHA::COMPASS direction = getNextCannonDirection();

	Vector3 launchPos = getCannonLaunchPosition(direction);
	Vector3 partialLaunchPoint(
		selfPos.x + ((launchPos.x - selfPos.x) * CANNON_LINE_FRACTION),
		selfPos.y + ((launchPos.y - selfPos.y) * CANNON_LINE_FRACTION),
		selfPos.z + ((launchPos.z - selfPos.z) * CANNON_LINE_FRACTION));

	DrawLineProjected(selfPos, partialLaunchPoint, WHITE);
}

Vector3 Sabetha::getCannonLaunchPosition(SABETHA::COMPASS direction) {
	return launchPositionMap[direction];
}

bool Sabetha::matchesTarget(Agent &agent) {
	if ((GetCurrentMapId() == 1062) && (agent.GetCharacter().GetMaxHealth() == MAX_HP)) {
		return true;
	}

	return false;
}

void Sabetha::updateState(boost::circular_buffer<float> &damageBuffer) {
	RaidBoss::updateState();
	RaidBoss::updateDps(damageBuffer);
}

void Sabetha::updateSquadState(SquadMemberMap &members) {

}

void Sabetha::drawAssistInfo() {
	stringstream ss;

	drawCompass();
	drawLineToNextCannon();

	drawToWindow(ss);
}

void Sabetha::outputDebug(stringstream &ss) {
	ss << format("agent valid: %s\n") % (agent.IsValid() ? "yes" : "no");
	ss << format("agentId: %d\n") % agent.GetAgentId();
	ss << format("agent ptr: %p\n") % &agent;
	ss << format("boss dead: %s\n") % (isDead() ? "yes" : "no");
	ss << format("elapsed: %d\n") % encounterTimer.getElapsedSeconds();
}