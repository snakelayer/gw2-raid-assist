#pragma once

#include "squad_member.h"

SquadMember::SquadMember(GW2LIB::Character character) :
	name(character.GetName()),
	dodgeCount(0),
	hitsTaken(0),
	totalDamageTaken(0.0f),
	isAlive(character.IsAlive()),
	lastEndurance(character.GetCurrentEndurance()),
	lastHealth(character.GetCurrentHealth()),
	lastHealthDelta(0.0f) {
}

void SquadMember::updateStats(GW2LIB::Character &character) {
	isAlive = character.IsAlive();
	updateLastHealthDelta(character);
	updateDamageTaken();
	updateDodgeCount(character);
}

void SquadMember::reset() {
	dodgeCount = 0;
	hitsTaken = 0;
	totalDamageTaken = 0.0f;
	lastEndurance = 0.0f;
	lastHealth = 0.0f;
	lastHealthDelta = 0.0f;
}

void SquadMember::updateLastHealthDelta(GW2LIB::Character &character) {
	float health = character.GetCurrentHealth();
	lastHealthDelta = health - lastHealth;
	lastHealth = health;
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
