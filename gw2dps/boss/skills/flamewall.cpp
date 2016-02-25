#include "flamewall.h"

using namespace boost;
using namespace GW2LIB;
using namespace std;

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

float Flamewall::getCooldown() {
	float remainingCooldown = -1.0f;
	if (state == FW::State::FIRST_CHARGE) {
		remainingCooldown = FIRST_COOLDOWN - getElapsedSeconds() + ACTIVATING_DURATION;
	}
	else if (state == FW::State::RECHARGING) {
		remainingCooldown = COOLDOWN - getElapsedSeconds() + ACTIVATING_DURATION;
	}
	else if (state == FW::State::ACTIVATING) {
		remainingCooldown = ACTIVATING_DURATION - getElapsedSeconds();
	}
	else if (state == FW::State::ACTIVE) {
		remainingCooldown = ACTIVE_DURATION - getElapsedSeconds();
	}
	else {
		return getElapsedSeconds();
	}
	
	return remainingCooldown;
}

bool Flamewall::tryUpdateRotation(float rotation) {
	if (startingRotation != rotation) {
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

void Flamewall::drawCooldownMeter(float x, float y) {
	float remainingCooldown = getCooldown();
	if (remainingCooldown < 0.0f) {
		return;
	}

	string cooldownStr;
	if (remainingCooldown < 5.0f) {
		cooldownStr = str(format("%.1f") % remainingCooldown);
	}
	else {
		cooldownStr = str(format("%d") % int(remainingCooldown));
	}

	float cooldownPercent = 0.0f;
	if (state == FW::State::FIRST_CHARGE) {
		cooldownPercent = remainingCooldown / (FIRST_COOLDOWN + ACTIVATING_DURATION);
	}
	else if (state == FW::State::RECHARGING || state == FW::State::ACTIVATING) {
		cooldownPercent = remainingCooldown / (COOLDOWN + ACTIVATING_DURATION);
	}
	else if (state == FW::State::ACTIVE) {
		cooldownPercent = remainingCooldown / ACTIVE_DURATION;
	}

	if (state == FW::State::FIRST_CHARGE || state == FW::State::RECHARGING || state == FW::State::ACTIVATING) {
		cooldownPercent = (1.0f - cooldownPercent);
	}

	meter.drawAtPercent(x, y, (state == FW::State::ACTIVE) ? ACTIVE : AssistDrawer::BREAKBAR_RECHARGING, cooldownStr, cooldownPercent); // TODO: change color
}

void Flamewall::drawActivatingMarker(Vector3 position) {
	float activatingPercent = getElapsedSeconds() / ACTIVATING_DURATION;
	drawFlamewallRect(position, flamewallLength, flamewallWidth, startingRotation, ACTIVATING_BASE);
	drawFlamewallRect(position, flamewallLength, flamewallWidth * activatingPercent, startingRotation, ACTIVATING_FILL);
}

void Flamewall::drawActiveMarker(Vector3 position) {
	drawFlamewallRect(position, flamewallLength, flamewallWidth, startingRotation, ACTIVE);
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

void Flamewall::drawFlamewallRect(Vector3 position, float length, float width, float rotation, DWORD color) {
	position.x += length * cos(rotation);
	position.y += length * sin(rotation);
	DrawRectFilledProjected(position, length, width, rotation, color);
}
