#include "seeker.h"

using namespace GW2LIB;

const float Seeker::MAX_HP = 649260;

Seeker::Seeker(Agent agent) : agent(agent), position(agent.GetPos()) {
}

int Seeker::getRespawnTime() {
	double secondsElapsed = timer.elapsed().wall / 1e9;
	return (int)(COOLDOWN - secondsElapsed);
}

Vector3 Seeker::getPosition() {
	if (agent.IsValid()) {
		position = agent.GetPos();
	}

	return position;
}

