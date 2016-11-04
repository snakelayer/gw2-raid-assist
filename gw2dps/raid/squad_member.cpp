#include "squad_member.h"

using namespace boost::assign;
using namespace GW2LIB;
using namespace std;

const float SquadMember::WALK_SPEED = 2.5f;
const float SquadMember::RUN_SPEED = 10.56562519073486328125f;
const float SquadMember::COMBAT_RUN_SPEED = 6.5625f;
const float SquadMember::DODGE_SPEED = 11.46875;
const float SquadMember::GLIDE_SPEED = 12.1875f;
const float SquadMember::SWIFTNESS_SPEED = 12.2193756103515625f;
const float SquadMember::COMBAT_SWIFTNESS_SPEED = 8.72812557220458984375f;
const float SquadMember::SUPERSPEED = 12.5f;

const double SquadMember::HEALTH_METER_FADE_DELAY_MILLISECONDS = 3000;

map<GW2::Race, float> SquadMember::raceHeightOffset = map_list_of
(GW2::Race::RACE_ASURA, 70.0f)
(GW2::Race::RACE_CHARR, 140.0f)
(GW2::Race::RACE_HUMAN, 90.0f)
(GW2::Race::RACE_NORN, 100.0f)
(GW2::Race::RACE_SYLVARI, 80.0f);

SquadMember::SquadMember(Player &player) :
    name(player.GetName()),
    profession(player.GetCharacter().GetProfession()),
    dodgeCount(0),
    superspeedCount(0),
    heavyHitsTaken(0),
    heavyDamageTaken(0.0f),
    totalDamageTaken(0.0f),
    directDamage(0),
    downedCount(0),
    lastSpeed(COMBAT_RUN_SPEED),
    secondLastSpeed(COMBAT_RUN_SPEED),
    isAlive(true),
    lastHealth(player.GetCharacter().GetCurrentHealth()),
    lastHealthDelta(0.0f),
    shouldDrawHealthMeter(false),
    meter(60.0f, 6.0f) {
    healthMeterTimer.stop();
}

void SquadMember::updateStats(Character &character) {
    updateLastHealthDelta(character);
    updateDamageTaken();
    updateMovementStats(character);

    if (isAlive && character.IsDowned()) {
        ++downedCount;
    }

    isAlive = character.IsAlive();
}

string SquadMember::getProfession() {
    switch (profession) {
    case GW2::Profession::PROFESSION_GUARDIAN:
        return "Guardian";
    case GW2::Profession::PROFESSION_WARRIOR:
        return "Warrior";
    case GW2::Profession::PROFESSION_ENGINEER:
        return "Engineer";
    case GW2::Profession::PROFESSION_RANGER:
        return "Ranger";
    case GW2::Profession::PROFESSION_THIEF:
        return "Thief";
    case GW2::Profession::PROFESSION_ELEMENTALIST:
        return "Elementalist";
    case GW2::Profession::PROFESSION_MESMER:
        return "Mesmer";
    case GW2::Profession::PROFESSION_NECROMANCER:
        return "Necromancer";
    case GW2::Profession::PROFESSION_REVENANT:
        return "Revenant";
    }

    return "Unknown";
}

void SquadMember::tryDrawHealthMeter(Character &character) {
    if (isBelowHalfHealth(character)) {
        shouldDrawHealthMeter = true;
        healthMeterTimer.start();
    }

    double elapsedMilliseconds = healthMeterTimer.elapsed().wall / 1e6;
    if ((elapsedMilliseconds > HEALTH_METER_FADE_DELAY_MILLISECONDS) ||
        !character.IsAlive()) {
        shouldDrawHealthMeter = false;
        healthMeterTimer.stop();
    }

    if (character.GetStance() == GW2::ProfessionStance::STANCE_NECRO_SHROUD) {
        return;
    }

    if (shouldDrawHealthMeter) {
        float healthPercent = lastHealth / character.GetMaxHealth();
        DWORD healthColor = interpolateHealthColor(healthPercent);
        meter.drawAtAgentPositionWithZOffset(character.GetAgent(), raceHeightOffset[character.GetRace()], healthColor, healthPercent);
    }
}

void SquadMember::takeHeavyHit() {
    heavyDamageTaken -= lastHealthDelta;
    ++heavyHitsTaken;
}

bool SquadMember::isBelowHalfHealth(Character &character) {
    return character.IsAlive() && (lastHealth < (character.GetMaxHealth() / 2));
}

DWORD SquadMember::interpolateHealthColor(float percent) {
    float half = 0.5f;
    DWORD red = (percent < half) ? 0xff : static_cast<int>(0xff * 2 * (1 - percent));
    DWORD green = static_cast<int>(0xff * percent);

    return 0xff000000 | (red << 16) | (green << 8);
}

void SquadMember::updateLastHealthDelta(Character &character) {
    float health = character.GetCurrentHealth();

    if (isAlive && character.IsAlive()) {
        lastHealthDelta = health - lastHealth;
        lastHealth = health;
    }
    else if (isAlive) {
        lastHealthDelta = -lastHealth;
        lastHealth = 0;
    }
    else if (character.IsAlive()) {
        lastHealthDelta = 0;
        lastHealth = health;
    }
    else {
        lastHealthDelta = 0;
    }
}

void SquadMember::updateDamageTaken() {
    if (isAlive && (lastHealthDelta < 0.0)) {
        totalDamageTaken -= lastHealthDelta;
    }
}

void SquadMember::updateMovementStats(Character &character) {
    float speed = character.GetAgent().GetSpeed();
    if (lastSpeed != DODGE_SPEED && lastSpeed != SUPERSPEED) {
        if (speed == DODGE_SPEED) {
            ++dodgeCount;
        }
        else if (speed == SUPERSPEED) {
            ++superspeedCount;
        }
    }
    else if (lastSpeed == DODGE_SPEED && speed == SUPERSPEED) {
        if (secondLastSpeed != SUPERSPEED) {
            ++superspeedCount;
        }
    }
    else if (lastSpeed == SUPERSPEED && speed == DODGE_SPEED) {
        ++dodgeCount;
    }

    if (speed != lastSpeed) {
        secondLastSpeed = lastSpeed;
    }
    lastSpeed = speed;
}
