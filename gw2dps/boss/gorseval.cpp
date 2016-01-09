#pragma once

#include "gorseval.h"

const float Gorseval::MAX_HP = 21628200;

Gorseval::Gorseval(Agent &agent) : RaidBoss(agent)
{
	// TODO
}

bool Gorseval::matchesTarget(Agent &agent) {
	if ((GetCurrentMapId() == 1062) && (agent.GetCharacter().GetMaxHealth() == MAX_HP)) {
		return true;
	}

	return false;
}

void Gorseval::updateState(boost::circular_buffer<float> &damageBuffer) {
	RaidBoss::updateState();
	updateDps(damageBuffer);

	// TODO: gorseval specific stuff
}

void Gorseval::outputAssistInfo(stringstream &ss) {
	// TODO
}