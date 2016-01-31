#pragma once

#include "sabetha.h"

const float Sabetha::MAX_HP = 34015256;

Sabetha::Sabetha(Agent agent) : RaidBoss(agent)
{
}

bool Sabetha::matchesTarget(Agent &agent) {
	if ((GetCurrentMapId() == 1062) && (agent.GetCharacter().GetMaxHealth() == MAX_HP)) {
		return true;
	}

	return false;
}

void Sabetha::updateState(boost::circular_buffer<float> &damageBuffer) {

}

void Sabetha::updateSquadState(SquadMemberMap &members) {

}

void Sabetha::outputAssistInfo(stringstream &ss) {

}

void Sabetha::outputDebug(stringstream &ss) {

}