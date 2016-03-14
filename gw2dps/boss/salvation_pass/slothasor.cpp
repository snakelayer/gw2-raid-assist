#include "slothasor.h"

using namespace boost;
using namespace GW2LIB;
using namespace std;

const float Slothasor::MAX_HP = 18973828;

Slothasor::Slothasor(Agent agent) : RaidBoss(agent)
{
    // ignores Volatile Aura
    // records Volatile Poison, Tantrum, Halitosis, Poison Mushroom, Spore Release
    heavyHitDamageThreshold = -2000.0f;
}

bool Slothasor::matchesTarget(Agent &agent) {
    return (GetCurrentMapId() == 1149) && (agent.GetCharacter().GetMaxHealth() == MAX_HP);
}

void Slothasor::updateState(circular_buffer<float> &damageBuffer) {
    RaidBoss::updateState();
    RaidBoss::updateDps(damageBuffer);

    if (encounterTimer.isStopped() && hasPlayerSlubling()) {
        startEncounter();
    }

    updateImbuedMushroom();
    updateVolatilePoison();
}

void Slothasor::drawAssistInfo() {
    stringstream ss;

    RaidBoss::drawAssistInfo();
    RaidBoss::outputAssistHeader(ss);
    outputSlublingCounts(ss);
    drawImbuedMushroomStatus();
    drawVolatilePoisonStatus();

    drawToWindow(ss, getDrawAssistPosition());
}

void Slothasor::outputDebug(stringstream &ss) {
    ss << format("encounterTimer: %d\n") % encounterTimer.getElapsedMilliseconds();
}

bool Slothasor::hasPlayerSlubling() {
    CharacterMap characterMap = squad->getCharacterMap();

    for (auto &characterEntry : characterMap) {
        if (characterEntry.second.GetAttitude() == GW2::Attitude::ATTITUDE_HOSTILE) {
            return true;
        }
    }

    return false;
}

void Slothasor::drawHealthTicks() {
    if (GetLockedSelection() != agent) {
        return;
    }

    float x = getHealthMeterX();
    float y = getHealthMeterY();

    DrawLine(x + HEALTHBAR_TICK_LENGTH / 10, y, x + HEALTHBAR_TICK_LENGTH / 10, y + HEALTHBAR_TICK_WIDTH, AssistDrawer::HEALTHBAR_TICK);
    DrawLine(x + HEALTHBAR_TICK_LENGTH / 5, y, x + HEALTHBAR_TICK_LENGTH / 5, y + HEALTHBAR_TICK_WIDTH, AssistDrawer::HEALTHBAR_TICK);
    DrawLine(x + HEALTHBAR_TICK_LENGTH * 2 / 5, y, x + HEALTHBAR_TICK_LENGTH * 2 / 5, y + HEALTHBAR_TICK_WIDTH, AssistDrawer::HEALTHBAR_TICK);
    DrawLine(x + HEALTHBAR_TICK_LENGTH / 2, y, x + HEALTHBAR_TICK_LENGTH / 2, y + HEALTHBAR_TICK_WIDTH, AssistDrawer::HEALTHBAR_TICK2);
    DrawLine(x + HEALTHBAR_TICK_LENGTH * 3 / 5, y, x + HEALTHBAR_TICK_LENGTH * 3 / 5, y + HEALTHBAR_TICK_WIDTH, AssistDrawer::HEALTHBAR_TICK);
    DrawLine(x + HEALTHBAR_TICK_LENGTH * 4 / 5, y, x + HEALTHBAR_TICK_LENGTH * 4 / 5, y + HEALTHBAR_TICK_WIDTH, AssistDrawer::HEALTHBAR_TICK);
}

void Slothasor::updateImbuedMushroom() {
    im.updateState(encounterTimer.getElapsedMilliseconds());
}

void Slothasor::drawImbuedMushroomStatus() {
    im.draw();
}

void Slothasor::outputSlublingCounts(stringstream &ss) {
    slubling.drawSlublingCounts(ss);
}

void Slothasor::updateVolatilePoison() {
    vp.updateState(encounterTimer.getElapsedMilliseconds());
}

void Slothasor::drawVolatilePoisonStatus() {
    Vector3 pos = getDrawAssistPosition();
    float x, y;

    if (getScreenLocation(&x, &y, pos)) {
        y += volatilePoisonDisplayOffset;
        vp.drawStatusMeter(x, y);
    }
}
