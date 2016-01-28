#pragma once

#include "seeker.h"

Seeker::Seeker() : state(SEEKER::INACTIVE) {
	timer.stop();
}

SEEKER::State Seeker::getState() {
	if ((state == SEEKER::INACTIVE) && (isSpawned())) {
		state = SEEKER::ACTIVE;
		timer.start();
	}
	else {
		if (!isSpawned()) {
			state = SEEKER::INACTIVE;
			timer.stop();
		}
		else if (getRespawnTime() <= 0.0f) {
			timer.start();
		}
	}
	return state;
}

int Seeker::getRespawnTime() {
	double secondsElapsed = timer.elapsed().wall / 1e9;
	return (int)(COOLDOWN - secondsElapsed);
}

bool Seeker::isSpawned() {
	GW2LIB::Agent agent;

	while (agent.BeNext()) {
		if (agent.GetCharacter().GetMaxHealth() == MAX_HP) {
			return true;
		}
	}

	return false;
}
