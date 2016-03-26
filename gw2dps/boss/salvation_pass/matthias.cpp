#include "matthias.h"

using namespace GW2LIB;
using namespace std;

const float Matthias::MAX_HP = 25953840.0f;

Matthias::Matthias(Agent agent) : RaidBoss(agent)
{
    heavyHitDamageThreshold = -0.0f; // TODO
}

bool Matthias::matchesTarget(Agent &agent) {
    return (GetCurrentMapId() == 1149) && (agent.GetCharacter().GetMaxHealth() == MAX_HP);
}

void Matthias::updateState(boost::circular_buffer<float> &damageBuffer) {
    RaidBoss::updateState();
    RaidBoss::updateDps(damageBuffer);

    // TODO
}

void Matthias::drawAssistInfo() {
    RaidBoss::drawAssistInfo();

    stringstream ss;
    RaidBoss::outputAssistHeader(ss);
    drawToWindow(ss, getDrawAssistPosition());
}

void Matthias::outputDebug(stringstream &ss) {
    // TODO
}

void Matthias::drawHealthTicks() {
    if (GetLockedSelection() != agent) {
        return;
    }

    float x = getHealthMeterX();
    float y = getHealthMeterY();

    DrawLine(x + HEALTHBAR_TICK_LENGTH * 2 / 5, y, x + HEALTHBAR_TICK_LENGTH * 2 / 5, y + HEALTHBAR_TICK_WIDTH, AssistDrawer::HEALTHBAR_TICK);
    DrawLine(x + HEALTHBAR_TICK_LENGTH * 3 / 5, y, x + HEALTHBAR_TICK_LENGTH * 3 / 5, y + HEALTHBAR_TICK_WIDTH, AssistDrawer::HEALTHBAR_TICK);
    DrawLine(x + HEALTHBAR_TICK_LENGTH * 4 / 5, y, x + HEALTHBAR_TICK_LENGTH * 4 / 5, y + HEALTHBAR_TICK_WIDTH, AssistDrawer::HEALTHBAR_TICK);
}
