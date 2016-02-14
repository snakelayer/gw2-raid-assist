#include "flamewall.h"

Flamewall::Flamewall() : state(FW::State::PENDING_START) {
	timer.stop();
}

FW::State Flamewall::getState() {
	if ((state == FW::State::FIRST_CHARGE) && !timer.is_stopped() && isFinishedFirstCharging()) {
		startActivating();
	}
	else if ((state == FW::State::RECHARGING) && !timer.is_stopped() && isFinishedRecharging()) {
		startActivating();
	}
	else if ((state == FW::State::ACTIVE) && !timer.is_stopped() && isFinishedAttacking()) {
		startRecharge();
	}

	return state;
}

bool Flamewall::tryUpdateRotation(float rotation) {
	if (startingRotation != rotation) { //TODO: gotta test this
		timer.start();
		startingRotation = rotation;

		return true;
	}

	return false;
}

void Flamewall::tryStartAttack() {
	if (isFinishedActivating()) {
		timer.start();
		state = FW::State::ACTIVE;
	}
}

void Flamewall::disable() {
	state = FW::State::PENDING_CENTER;
	timer.stop();
}

void Flamewall::startActivating() {
	state = FW::State::ACTIVATING;
	timer.start();
}

void Flamewall::startFirstCharge() {
	state = FW::State::FIRST_CHARGE;
	timer.start();
}

void Flamewall::startRecharge() {
	state = FW::State::RECHARGING;
	timer.start();
}

bool Flamewall::isFinishedFirstCharging() {
	if (getElapsedSeconds() >= FIRST_COOLDOWN) {
		return true;
	}

	return false;
}

bool Flamewall::isFinishedRecharging() {
	if (getElapsedSeconds() >= COOLDOWN) {
		return true;
	}

	return false;
}

bool Flamewall::isFinishedActivating() {
	if (getElapsedSeconds() >= ACTIVATING_DURATION) {
		return true;
	}

	return false;
}

bool Flamewall::isFinishedAttacking() {
	if (getElapsedSeconds() >= ACTIVE_DURATION) { // could also check rotation, if we need it
		return true;
	}

	return false;
}

float Flamewall::getElapsedSeconds() {
	return float(timer.elapsed().wall / 1e9);
}
