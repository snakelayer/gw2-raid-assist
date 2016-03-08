#include "slothasor.h"

using namespace boost;
using namespace GW2LIB;
using namespace std;

const float Slothasor::MAX_HP = 18973828;

Slothasor::Slothasor(Agent agent) : RaidBoss(agent)
{
    healthMarker = RB::HEALTH_MARKER::FIFTHS;
    heavyHitDamageThreshold = -3000.0f; // TODO: adjust
}

bool Slothasor::matchesTarget(Agent &agent) {
    if ((GetCurrentMapId() == 1149) && (agent.GetCharacter().GetMaxHealth() == MAX_HP)) {
        return true;
    }

    return false;
}

void Slothasor::updateState(circular_buffer<float> &damageBuffer) {
    RaidBoss::updateState();
    RaidBoss::updateDps(damageBuffer);
}

void Slothasor::drawAssistInfo() {
    stringstream ss;

    RaidBoss::drawAssistInfo();
    RaidBoss::outputAssistHeader(ss);
    drawToWindow(ss, getDrawAssistPosition());
}

void Slothasor::outputDebug(stringstream &ss) {

}
