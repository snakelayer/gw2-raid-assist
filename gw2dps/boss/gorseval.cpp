#include "gorseval.h"

using namespace boost;
using namespace GW2LIB;
using namespace std;

const float Gorseval::MAX_HP = 21628200;

Gorseval::Gorseval(Agent agent) : RaidBoss(agent)
{
    heavyHitDamageThreshold = -1500.0f;
}

void Gorseval::updateGhastlyRampage() {
    ghastlyRampage.update(agent.GetCharacter().GetBreakbarState(), getBreakbar());
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

void Gorseval::drawGhastlyRampageStatus() {
    if (ghastlyRampage.getState() == GR::State::PENDING) {
        return;
    }

    Vector3 pos = getDrawAssistPosition();
    float x, y;

    if (getScreenLocation(&x, &y, pos)) {
        y += ghastlyRampageDisplayOffset;
        ghastlyRampage.draw(x, y);
    }
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

    updateGhastlyRampage();
}

void Gorseval::drawAssistInfo() {
    RaidBoss::drawAssistInfo();

    stringstream ss;
    RaidBoss::outputAssistHeader(ss);

    drawGhastlyRampageStatus();

    drawToWindow(ss, getDrawAssistPosition());
}

void Gorseval::outputDebug(stringstream &ss) {
    ss << outputHeader;
    ss << format("encounter timer: %d\n") % encounterTimer.getElapsedMilliseconds();
    //outputHeader = "";
}
