#include "squad_member.h"

const float SquadMember::SWIFTNESS_THRESHOLD = 22.0f;
const float SquadMember::DODGE_THRESHOLD = 31.0f;
const float SquadMember::SUPERSPEED_THRESHOLD = 38.0f;

SquadMember::SquadMember(GW2LIB::Character character) :
	name(character.GetName()),
	dodgeCount(0),
	heavyHitsTaken(0),
	heavyDamageTaken(0.0f),
	totalDamageTaken(0.0f),
	downedCount(0),
	speedState(SM::SPEED_STATE::BASIC),
	isAlive(character.IsAlive()),
	lastEndurance(character.GetCurrentEndurance()),
	lastHealth(character.GetCurrentHealth()),
	lastHealthDelta(0.0f) {
	dodgeTimer.stop();
}

void SquadMember::updateStats(GW2LIB::Character &character) {
	updateLastHealthDelta(character);
	updateDamageTaken();
	//updateDodgeCount(character);

	if (isAlive && character.IsDowned()) {
		++downedCount;
	}

	isAlive = character.IsAlive();
}

void SquadMember::inferDodgeStateWithSpeed(float speed) {
	if (speedState == SM::SPEED_STATE::BASIC) {
		if (speed > SUPERSPEED_THRESHOLD) {
			// should not happen
			speedState = SM::SPEED_STATE::SUPERSPEED;
		}
		else if (speed > DODGE_THRESHOLD) {
			speedState = SM::SPEED_STATE::DODGE;
			tryUpdateDodgeCount();
		}
		else if (speed > SWIFTNESS_THRESHOLD) {
			speedState = SM::SPEED_STATE::SWIFTNESS;
		}
	}
	else if (speedState == SM::SPEED_STATE::SWIFTNESS) {
		if (speed > SUPERSPEED_THRESHOLD) {
			//should not happen
			speedState = SM::SPEED_STATE::SUPERSPEED;
		}
		else if (speed > DODGE_THRESHOLD) {
			speedState = SM::SPEED_STATE::DODGE;
			tryUpdateDodgeCount();
		}
		else if (speed > SWIFTNESS_THRESHOLD) {
		}
		else {
			speedState = SM::SPEED_STATE::BASIC;
		}
	}
	else if (speedState == SM::SPEED_STATE::DODGE) {
		if (speed > SUPERSPEED_THRESHOLD) {
			speedState = SM::SPEED_STATE::SUPERSPEED;
			//tryUpdateDodgeCount(); // this is too unreliable
		}
		else if (speed > DODGE_THRESHOLD) {
			tryUpdateDodgeCount();
		}
		else if (speed > SWIFTNESS_THRESHOLD) {
			speedState = SM::SPEED_STATE::SWIFTNESS;
		}
		else {
			speedState = SM::SPEED_STATE::BASIC;
		}
	}
	else if (speedState == SM::SPEED_STATE::SUPERSPEED) {
		if (speed > SUPERSPEED_THRESHOLD) {
		}
		else if (speed > DODGE_THRESHOLD) {
			speedState = SM::SPEED_STATE::DODGE;
		}
		else if (speed > SWIFTNESS_THRESHOLD) {
			speedState = SM::SPEED_STATE::SWIFTNESS;
		}
		else {
			speedState = SM::SPEED_STATE::BASIC;
		}
	}
}

void SquadMember::takeHeavyHit() {
	heavyDamageTaken -= lastHealthDelta;
	++heavyHitsTaken;
}

void SquadMember::updateLastHealthDelta(GW2LIB::Character &character) {
	float health = character.GetCurrentHealth();

	if (isAlive && character.IsAlive()) {
		lastHealthDelta = health - lastHealth;
		lastHealth = health;
	}
	else if (isAlive) {
		lastHealthDelta = -lastHealth;
		lastHealth = 0;
	}
	else if (character.IsAlive()) {
		lastHealthDelta = 0;
		lastHealth = health;
	}
	else {
		lastHealthDelta = 0;
	}
}

void SquadMember::updateDamageTaken() {
	if (isAlive && (lastHealthDelta < 0.0)) {
		totalDamageTaken -= lastHealthDelta;
	}
}

void SquadMember::updateDodgeCount(GW2LIB::Character &character) {
	float endurance = character.GetCurrentEndurance();
	if (endurance < lastEndurance) {
		dodgeCount++;
	}

	lastEndurance = endurance;
}

void SquadMember::tryUpdateDodgeCount() {
	if (speedState == SM::SPEED_STATE::SUPERSPEED) {
		dodgeTimer.stop();
		--dodgeCount;
	}
	else {
		if (!dodgeTimer.is_stopped()) { // recently already updated
			if ((dodgeTimer.elapsed().wall / 1e6) > 800.0f) {
				dodgeTimer.start();
				++dodgeCount;
			}
		}
		else {
			dodgeTimer.start();
			++dodgeCount;
		}
	}
}
