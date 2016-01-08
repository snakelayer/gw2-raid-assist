#ifndef SQUAD_MEMBER_CPP
#define SQUAD_MEMBER_CPP

#include "squad_member.h"

SquadMember::SquadMember(GW2LIB::Character character) :
	name(character.GetName()),
	dodgeCount(0),
	totalDamageTaken(0.0f),
	lastEndurance(character.GetCurrentEndurance()),
	lastHealth(character.GetCurrentHealth()) {
}

void SquadMember::updateStats(GW2LIB::Character &character) {
	updateTotalDamageTaken(character);
	updateDodgeCount(character);
}

void SquadMember::reset() {
	dodgeCount = 0;
	totalDamageTaken = 0.0f;
	lastEndurance = 0.0f;
	lastHealth = 0.0f;
}

void SquadMember::updateTotalDamageTaken(GW2LIB::Character &character) {
	if (!character.IsAlive()) {
		return;
	}
	float health = character.GetCurrentHealth();
	if (health < lastHealth) {
		totalDamageTaken += lastHealth - health;
	}

	lastHealth = health;
}

void SquadMember::updateDodgeCount(GW2LIB::Character &character) {
	float endurance = character.GetCurrentEndurance();
	if (endurance < lastEndurance) {
		dodgeCount++;
	}

	lastEndurance = endurance;
}

#endif