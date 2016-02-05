#include "gw2lib.h"

#include "unknown_boss.h"

UnknownBoss::UnknownBoss(Agent agent) : RaidBoss(agent) {

}

void UnknownBoss::updateState(boost::circular_buffer<float> &damageBuffer) {
	RaidBoss::updateState();
	RaidBoss::updateDps(damageBuffer);
}

void UnknownBoss::updateSquadState(SquadMemberMap &members) {

}

void UnknownBoss::outputDps(stringstream &ss) {

}

void UnknownBoss::outputAssistInfo(stringstream &ss) {
	RaidBoss::outputAssistHeader(ss);
}

int UnknownBoss::getEncounterTime() {
	return 0;
}

void UnknownBoss::outputDebug(stringstream &ss) {
	//ss << outputHeader;
}