#ifndef UNKNOWN_BOSS_CPP
#define UNKNOWN_BOSS_CPP

#include "gw2lib.h"

#include "unknown_boss.h"

UnknownBoss::UnknownBoss(Agent &agent) : agent(&agent) {

}

bool UnknownBoss::matchesTarget(Agent &agent) {
	return false;
}

void UnknownBoss::updateState(boost::circular_buffer<float> &damageBuffer) {

}

void UnknownBoss::outputDps(stringstream &ss) {

}

void UnknownBoss::outputAssistInfo(stringstream &ss) {
	ss << format("Boss: %s\n") % getName();
}

int UnknownBoss::getEncounterTime() {
	return 0;
}

#endif