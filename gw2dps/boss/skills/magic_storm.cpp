#pragma once

#include "magic_storm.h"

MagicStorm::MagicStorm() : state(MS::PENDING) {
	timer.stop(); // TODO: check if we need to instantiate
}

MS::State MagicStorm::getState() {
	if ((state == MS::RECHARGING) && !timer.is_stopped() && (getCooldown() <= 0.0f)) {
		timer.stop();
		state = MS::READY;
	}
	return state;
}

void MagicStorm::setState(MS::State state) {
	this->state = state;
	if (state == MS::RECHARGING) {
		timer.start();
	}
	else if (!timer.is_stopped()) {
		timer.stop();
	}
}
int MagicStorm::getCooldown() {
	double secondsElapsed = timer.elapsed().wall / 1e9;
	return (int)(COOLDOWN - secondsElapsed);
}
