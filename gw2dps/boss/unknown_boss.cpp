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
	ss << format("Boss: %s\n") % getName();
}

int UnknownBoss::getEncounterTime() {
	return 0;
}

void UnknownBoss::outputDebug(stringstream &ss) {
	ss << debugStr;
}