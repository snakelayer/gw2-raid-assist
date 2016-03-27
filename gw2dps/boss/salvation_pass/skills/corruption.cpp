#include "corruption.h"

using namespace boost;
using namespace boost::assign;
using namespace GW2LIB;
using namespace std;

const float Corruption::FONT_RADIUS = 160.0f; // real value might be closer to 150

map<CORRUPTION::DIRECTION, Vector3> Corruption::positionMap = map_list_of
(CORRUPTION::DIRECTION::SW, Vector3(-6834, 4994, -5189))
(CORRUPTION::DIRECTION::NW, Vector3(-6851, 6801, -5181))
(CORRUPTION::DIRECTION::NE, Vector3(-5033, 6817, -5190))
(CORRUPTION::DIRECTION::SE, Vector3(-5021, 5000, -5181));

Corruption::Corruption() : elapsedMilliseconds(0) {
    cooldownMap[CORRUPTION::DIRECTION::SW].stop();
    cooldownMap[CORRUPTION::DIRECTION::NW].stop();
    cooldownMap[CORRUPTION::DIRECTION::NE].stop();
    cooldownMap[CORRUPTION::DIRECTION::SE].stop();
}

void Corruption::updateState(int elapsedMilliseconds) {
    this->elapsedMilliseconds = elapsedMilliseconds;
    updateCleansingFonts();
}

void Corruption::drawCleansingFonts() {
    stringstream ss;
    float x, y;

    for (auto &positionEntry : positionMap) {
        CORRUPTION::DIRECTION direction = positionEntry.first;

        ss.str("");
        if (cooldownMap[direction].is_stopped()) {
            ss << "ready";
        }
        else {
            ss << format("%d") % getFontCooldown(direction);
        }

        if (WorldToScreen(positionEntry.second, &x, &y)) {
            AssistDrawer::get().drawStreamToWindow(ss, x, y);
        }
    }
}

void Corruption::drawMeter(float x, float y) {
    float skillCooldown = getSkillCooldown();
    float percent = (1.0f - skillCooldown / (SKILL_COOLDOWN / 1000.0f));

    meter.drawAtPercent(x, y, AssistDrawer::SKILL_RECHARGING, skillCooldown, percent);
}

bool Corruption::isInFont(CORRUPTION::DIRECTION direction, Vector3 pos) {
    Vector3 fontPos = positionMap[direction];
    return sqrt(pow(pos.x - fontPos.x, 2) + pow(pos.y - fontPos.y, 2) + pow(pos.z - fontPos.z, 2)) < FONT_RADIUS;
}

bool Corruption::isFontReady(CORRUPTION::DIRECTION direction) {
    return (cooldownMap[direction].elapsed().wall / 1e9) >= FONT_COOLDOWN;
}

int Corruption::getFontCooldown(CORRUPTION::DIRECTION direction) {
    return FONT_COOLDOWN - int(cooldownMap[direction].elapsed().wall / 1e9);
}

float Corruption::getSkillCooldown() {
    return (SKILL_COOLDOWN - ((elapsedMilliseconds + SKILL_START_OFFSET) % SKILL_COOLDOWN)) / 1000.0f;
}

void Corruption::updateCleansingFonts() {
    for (auto &cooldownEntry : cooldownMap) {
        if (isFontReady(cooldownEntry.first)) {
            cooldownEntry.second.stop();
        }
    }

    // TODO: if this is too slow use a direct player list
    Agent agent;
    while (agent.BeNext()) {
        if (!agent.IsValid() || !agent.GetCharacter().IsPlayer()) {
            continue;
        }

        for (auto &positionEntry : positionMap) {
            CORRUPTION::DIRECTION direction = positionEntry.first;

            if (isInFont(direction, agent.GetPos())) {
                if (cooldownMap[direction].is_stopped()) {
                    cooldownMap[direction].start();
                }
            }
        }
    }
}
