#include "squad_member.h"

using namespace GW2LIB;

const float SquadMember::WALK_SPEED = 2.5f;
const float SquadMember::RUN_SPEED = 10.56562519073486328125f;
const float SquadMember::COMBAT_RUN_SPEED = 6.5625f;
const float SquadMember::DODGE_SPEED = 11.46875;
const float SquadMember::GLIDE_SPEED = 12.1875f;
const float SquadMember::SWIFTNESS_SPEED = 12.2193756103515625f;
const float SquadMember::COMBAT_SWIFTNESS_SPEED = 8.72812557220458984375f;
const float SquadMember::SUPERSPEED = 12.5f;

SquadMember::SquadMember(Player player) :
    name(player.GetName()),
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
    lastHealthDelta(0.0f) {
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

void SquadMember::takeHeavyHit() {
    heavyDamageTaken -= lastHealthDelta;
    ++heavyHitsTaken;
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
