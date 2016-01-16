#pragma once

#include "gw2lib.h"

#include "unknown_boss.h"

UnknownBoss::UnknownBoss(Agent agent) : RaidBoss(agent) {

}

void UnknownBoss::updateState(boost::circular_buffer<float> &damageBuffer) {
}

void UnknownBoss::updateSquadState(Squad *squad) {

}

void UnknownBoss::outputDps(stringstream &ss) {

}

void UnknownBoss::outputAssistInfo(stringstream &ss) {
	outputAssistHeader(ss);
}

int UnknownBoss::getEncounterTime() {
	return 0;
}

void UnknownBoss::outputDebug(stringstream &ss) {
	debugStr = str(format("current hp: %f\n") % agent.GetCharacter().GetCurrentHealth());
	ss << debugStr;
}