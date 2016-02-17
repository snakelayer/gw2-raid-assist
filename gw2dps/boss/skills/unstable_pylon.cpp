#include "unstable_pylon.h"

UnstablePylon::UnstablePylon() : state(UP::State::PENDING_SPLIT) {
	timer.stop();
}

void UnstablePylon::startWith(UP::State state) {
	timer.start();
	this->state = state;
}

void UnstablePylon::update() {
	if (!timer.is_stopped()) {
		if (state == UP::State::PENDING_MOVE_FIRST && (getMoveCooldown() <= 0.0f)) {
			state = UP::State::SAFE_RED_BLUE;
			timer.start();
		}
		else if (state == UP::State::PENDING_MOVE_SECOND && (getMoveCooldown() <= 0.0f)) {
			state = UP::State::SAFE_RED;
			timer.start();
		}
		else if (isFirst() && getFirstCooldown() <= 0.0f) {
			tick();
		}
		else if (isSecond() && getSecondCooldown() <= 0.0f) {
			tick();
		}
	}
}

void UnstablePylon::stop() {
	state = UP::State::PENDING_SPLIT;
	timer.stop();
}

bool UnstablePylon::isFirst() {
	return state == UP::State::SAFE_RED_BLUE ||
		state == UP::State::SAFE_GREEN_RED ||
		state == UP::State::SAFE_BLUE_GREEN;
}

bool UnstablePylon::isSecond() {
	return state == UP::State::SAFE_RED ||
		state == UP::State::SAFE_GREEN ||
		state == UP::State::SAFE_BLUE;
}

void UnstablePylon::tick() {
	if (state == UP::State::SAFE_RED_BLUE) {
		state = UP::State::SAFE_GREEN_RED;
	}
	else if (state == UP::State::SAFE_GREEN_RED) {
		state = UP::State::SAFE_BLUE_GREEN;
	}
	else if (state == UP::State::SAFE_BLUE_GREEN) {
		state = UP::State::SAFE_RED_BLUE;
	}
	else if (state == UP::State::SAFE_RED) {
		state = UP::State::SAFE_GREEN;
	}
	else if (state == UP::State::SAFE_GREEN) {
		state = UP::State::SAFE_BLUE;
	}
	else if (state == UP::State::SAFE_BLUE) {
		state = UP::State::SAFE_RED;
	}

	timer.start();
}

float UnstablePylon::getFirstCooldown() {
	float secondsElapsed = float(timer.elapsed().wall / 1e9);
	return FIRST_COOLDOWN - secondsElapsed;
}

float UnstablePylon::getSecondCooldown() {
	float secondsElapsed = float(timer.elapsed().wall / 1e9);
	return SECOND_COOLDOWN - secondsElapsed;
}

float UnstablePylon::getMoveCooldown() {
	float secondsElapsed = float(timer.elapsed().wall / 1e9);
	return MOVE_COOLDOWN - secondsElapsed;
}
