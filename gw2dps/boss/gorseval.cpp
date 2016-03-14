#include "gorseval.h"

using namespace GW2LIB;
using namespace std;

const float Gorseval::MAX_HP = 21628200;

Gorseval::Gorseval(Agent agent) : RaidBoss(agent)
{
    heavyHitDamageThreshold = -1500.0f;
}

void Gorseval::drawHealthTicks() {
    if (GetLockedSelection() != agent) {
        return;
    }

    float x = getHealthMeterX();
    float y = getHealthMeterY();

    DrawLine(x + HEALTHBAR_TICK_LENGTH / 3, y, x + HEALTHBAR_TICK_LENGTH / 3, y + HEALTHBAR_TICK_WIDTH, AssistDrawer::HEALTHBAR_TICK);
    DrawLine(x + HEALTHBAR_TICK_LENGTH * 2 / 3, y, x + HEALTHBAR_TICK_LENGTH * 2 / 3, y + HEALTHBAR_TICK_WIDTH, AssistDrawer::HEALTHBAR_TICK);
}

bool Gorseval::matchesTarget(Agent &agent) {
    return (GetCurrentMapId() == 1062) && (agent.GetCharacter().GetMaxHealth() == MAX_HP);
}

void Gorseval::updateState(boost::circular_buffer<float> &damageBuffer) {
    RaidBoss::updateState();
    RaidBoss::updateDps(damageBuffer);

    if (agent.m_ptr == nullptr) {
        return;
    }

    // TODO: gorseval specific stuff
}

void Gorseval::drawAssistInfo() {
    RaidBoss::drawAssistInfo();

    stringstream ss;
    RaidBoss::outputAssistHeader(ss);
    // TODO

    drawToWindow(ss, getDrawAssistPosition());
}

void Gorseval::outputDebug(stringstream &ss) {
    ss << outputHeader;
    //outputHeader = "";
}
