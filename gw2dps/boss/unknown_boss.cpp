#include "unknown_boss.h"

using namespace GW2LIB;
using namespace std;

UnknownBoss::UnknownBoss(GW2LIB::Agent agent) : RaidBoss(agent) {

}

void UnknownBoss::updateState(boost::circular_buffer<float> &damageBuffer) {
    RaidBoss::updateState();
    RaidBoss::updateDps(damageBuffer);
}

void UnknownBoss::outputDps(stringstream &ss) {

}

void UnknownBoss::drawAssistInfo() {
    RaidBoss::drawAssistInfo();

    stringstream ss;
    RaidBoss::outputAssistHeader(ss);

    drawToWindow(ss, getDrawAssistPosition());
}

int UnknownBoss::getEncounterTime() {
    return 0;
}

void UnknownBoss::outputDebug(stringstream &ss) {
    //ss << outputHeader;
}