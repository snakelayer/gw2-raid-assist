#include "sacrifice.h"

using namespace GW2LIB;

const float Sacrifice::PLAYER_HEIGHT_OFFSET = 160.0f;

Sacrifice::Sacrifice() {
    timer.stop();
}

void Sacrifice::updateState(Agent bossAgent) {
    if (sacrificeTarget.IsValid()) {
        updateSacrificeTarget(bossAgent);
        return;
    }

    if (isSelfSacrifice(bossAgent)) {
        setTarget(GetOwnAgent());
        return;
    }

    Agent agent;
    while (agent.BeNext()) {
        if (isAllySacrifice(agent)) {
            setTarget(agent);
            return;
        }
    }
}

int Sacrifice::getRemainingDuration() {
    return DURATION - int(timer.elapsed().wall / 1e6);
}

void Sacrifice::draw() {
    if (!sacrificeTarget.IsValid()) {
        return;
    }

    Vector3 pos = sacrificeTarget.GetPos();
    float x, y;
    if (!WorldToScreen(pos, &x, &y)) {
        return;
    }
    y -= PLAYER_HEIGHT_OFFSET;

    float remainingDuration = getRemainingDuration() / 1000.0f;
    float percent = float(getRemainingDuration()) / DURATION;

    meter.drawAtPercent(x, y, AssistDrawer::BLACK, remainingDuration, percent);
}

bool Sacrifice::isSelfSacrifice(Agent bossAgent) {
    Character character = bossAgent.GetCharacter();
    return character.IsValid() && (character.GetAttitude() == GW2::Attitude::ATTITUDE_FRIENDLY);
}

bool Sacrifice::isAllySacrifice(Agent allyAgent) {
    Character character = allyAgent.GetCharacter();
    return allyAgent.IsValid() && character.IsValid() && character.IsPlayer() && (character.GetAttitude() == GW2::Attitude::ATTITUDE_HOSTILE);
}

void Sacrifice::setTarget(Agent agent) {
    timer.start();
    sacrificeTarget = agent;
}

void Sacrifice::clearTarget() {
    timer.stop();
    sacrificeTarget.m_ptr = nullptr;
}

void Sacrifice::updateSacrificeTarget(Agent bossAgent) {
    if (timer.is_stopped()) {
        return;
    }

    if (getRemainingDuration() <= 0) {
        clearTarget();
    }

    if (sacrificeTarget == GetOwnAgent()) {
        if (!isSelfSacrifice(bossAgent)) {
            clearTarget();
        }
    }
    else {
        if (!isAllySacrifice(sacrificeTarget)) {
            clearTarget();
        }
    }
}
