#include "squad_member.h"

using namespace boost::assign;
using namespace boost::chrono;
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

const float SquadMember::SCHOLAR_HEALTH_PERCENT_THRESHOLD = 0.9f;

const double SquadMember::GOTL_SAMPLES_PER_STACK = 8.0f * 4; // 8 sec duration, 250ms sampling rate

unordered_set<GW2::EffectType> SquadMember::FOOD_BUFFS = list_of
(GW2::EffectType::EFFECT_FOOD_100_POWER_70_CONDI_DMG)
(GW2::EffectType::EFFECT_FOOD_80_POWER_60_PRECISION)
(GW2::EffectType::EFFECT_FOOD_100_POWER_70_PRECISION)
(GW2::EffectType::EFFECT_FOOD_80_PRECISION_60_FEROCITY)
(GW2::EffectType::EFFECT_FOOD_100_PRECISION_70_FEROCITY)
(GW2::EffectType::EFFECT_FOOD_40_MF_WITH_BOON_20_BOON_DURATION)
(GW2::EffectType::EFFECT_FOOD_20_CONDI_DURATION_70_CONDI_DMG)
(GW2::EffectType::EFFECT_FOOD_50_SWIFTNESS_ON_KILL_8_MOVING_DMG)
(GW2::EffectType::EFFECT_FOOD_60_SWIFTNESS_ON_KILL_10_MOVING_DMG)
(GW2::EffectType::EFFECT_FOOD_100_POWER_70_FEROCITY)
(GW2::EffectType::EFFECT_FOOD_70_TOUGHNESS_20_BOON_DURATION)
(GW2::EffectType::EFFECT_FOOD_20_CONDI_DURATION_33_LIFESTEAL_ON_CRIT)
(GW2::EffectType::EFFECT_FOOD_20_BOON_DURATION_25_MF_DURING_LNY_33_MIGHT_ON_CRIT)
(GW2::EffectType::EFFECT_FOOD_20_BOON_DURATION_25_MF_DURING_LNY_100_SWIFTNESS_ON_KILL);

unordered_set<GW2::EffectType> SquadMember::UTILITY_BUFFS = list_of
(GW2::EffectType::EFFECT_UTILITY_SHARPENING_STONE_ROUGH)
(GW2::EffectType::EFFECT_UTILITY_SHARPENING_STONE_SIMPLE)
(GW2::EffectType::EFFECT_UTILITY_SHARPENING_STONE_STANDARD)
(GW2::EffectType::EFFECT_UTILITY_SHARPENING_STONE_QUALITY)
(GW2::EffectType::EFFECT_UTILITY_SHARPENING_STONE_HARDENED)
(GW2::EffectType::EFFECT_UTILITY_SHARPENING_STONE_SUPERIOR)
(GW2::EffectType::EFFECT_UTILITY_SHARPENING_STONE_BOUNTIFUL)
(GW2::EffectType::EFFECT_UTILITY_SHARPENING_STONE_FURIOUS)
(GW2::EffectType::EFFECT_UTILITY_SHARPENING_STONE_TOXIC)
(GW2::EffectType::EFFECT_UTILITY_TUNING_CRYSTAL_APPRENTICE)
(GW2::EffectType::EFFECT_UTILITY_TUNING_CRYSTAL_JOURNEYMAN)
(GW2::EffectType::EFFECT_UTILITY_TUNING_CRYSTAL_STANDARD)
(GW2::EffectType::EFFECT_UTILITY_TUNING_CRYSTAL_ARTISAN)
(GW2::EffectType::EFFECT_UTILITY_TUNING_CRYSTAL_QUALITY)
(GW2::EffectType::EFFECT_UTILITY_TUNING_CRYSTAL_MASTER)
(GW2::EffectType::EFFECT_UTILITY_TUNING_CRYSTAL_BOUNTIFUL)
(GW2::EffectType::EFFECT_UTILITY_TUNING_CRYSTAL_FURIOUS)
(GW2::EffectType::EFFECT_UTILITY_FOCUSING_CRYSTAL_TOXIC)
(GW2::EffectType::EFFECT_UTILITY_MAINTENANCE_OIL_APPRENTICE)
(GW2::EffectType::EFFECT_UTILITY_MAINTENANCE_OIL_JOURNEYMAN)
(GW2::EffectType::EFFECT_UTILITY_MAINTENANCE_OIL_STANDARD)
(GW2::EffectType::EFFECT_UTILITY_MAINTENANCE_OIL_ARTISAN)
(GW2::EffectType::EFFECT_UTILITY_MAINTENANCE_OIL_QUALITY)
(GW2::EffectType::EFFECT_UTILITY_MAINTENANCE_OIL_MASTER)
(GW2::EffectType::EFFECT_UTILITY_MAINTENANCE_OIL_BOUNTIFUL)
(GW2::EffectType::EFFECT_UTILITY_MAINTENANCE_OIL_FURIOUS)
(GW2::EffectType::EFFECT_UTILITY_MAINTENANCE_OIL_TOXIC);

const double SquadMember::HEALTH_METER_FADE_DELAY_MILLISECONDS = 3000;

map<GW2::Race, float> SquadMember::RACE_HEIGHT_OFFSET = map_list_of
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
    directDamageOutput(0),
    downedCount(0),
    lastSpeed(COMBAT_RUN_SPEED),
    secondLastSpeed(COMBAT_RUN_SPEED),
    isAlive(true),
    lastHealth(player.GetCharacter().GetCurrentHealth()),
    lastHealthDelta(0.0f),
    shouldDrawHealthMeter(false),
    meter(60.0f, 6.0f),
    totalUptime(milliseconds(0)),
    alacrityDuration(milliseconds(0)),
    sumMight(0),
    mightSamples(0),
    sumFury(0),
    furySamples(0),
    sumScholarly(0),
    scholarlySamples(0),
    sumQuickness(0),
    quicknessSamples(0),
    sumGotl(0) {
    healthMeterTimer.stop();
    uptimeTimer.stop();
    alacrityTimer.stop();
}

void SquadMember::updateStats(Character &character) {
    updateLastHealthDelta(character);
    updateDamageTaken();
    updateMovementStats(character);

    addMight(character.GetBuffStackCount(GW2::EffectType::EFFECT_MIGHT));
    addFury(character.GetBuffStackCount(GW2::EffectType::EFFECT_FURY));
    addScholarly(character);
    addQuickness(character.GetBuffStackCount(GW2::EffectType::EFFECT_QUICKNESS));
    updateAlacrity(character.GetBuffStackCount(GW2::EffectType::EFFECT_ALACRITY) != 0);
    addGotl(character.GetBuffStackCount(GW2::EffectType::EFFECT_GRACE_OF_THE_LAND));

    if (isAlive && character.IsDowned()) {
        ++downedCount;
    }

    if (character.IsAlive()) {
        upped();
    }
    else {
        downed();
    }

    isAlive = character.IsAlive();
}

void SquadMember::stopTimers() {
    updateUptime();
    updateAlacrity(false);
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

void SquadMember::drawNourishmentCheck(Character &character) {
    string reminder;

    if (!hasFood(character)) {
        reminder += "food?";
    }

    if (!hasUtility(character)) {
        if (!reminder.empty()) {
            reminder += "/";
        }
        reminder += "utility?";
    }

    if (!reminder.empty()) {
        AssistDrawer::get().drawFont(character.GetAgent(), AssistDrawer::WHITE, reminder);
    }
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
        meter.drawAtAgentPositionWithZOffset(character.GetAgent(), RACE_HEIGHT_OFFSET[character.GetRace()], healthColor, healthPercent);
    }
}

void SquadMember::takeHeavyHit() {
    heavyDamageTaken -= lastHealthDelta;
    ++heavyHitsTaken;
}

void SquadMember::addMight(int stacks) {
    sumMight += stacks;
    mightSamples += 1;
}

void SquadMember::addFury(int stacks) {
    sumFury += (stacks != 0);
    furySamples += 1;
}

void SquadMember::addScholarly(GW2LIB::Character &character) {
    if (character.IsAlive() &&
        (character.GetCurrentHealth() > character.GetMaxHealth() * SCHOLAR_HEALTH_PERCENT_THRESHOLD)) {
        sumScholarly += 1;
    }
    scholarlySamples += 1;
}

void SquadMember::addQuickness(int stacks) {
    sumQuickness += (stacks != 0);
    quicknessSamples += 1;
}

void SquadMember::addGotl(int stacks) {
    sumGotl += stacks;
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

bool SquadMember::hasFood(Character &character) {
    Buff buffs = character.GetBuffs();
    while (buffs.BeNext()) {
        if (FOOD_BUFFS.find(buffs.GetEffectType()) != FOOD_BUFFS.end()) {
            return true;
        }
    }
    return false;
}

bool SquadMember::hasUtility(Character &character) {
    Buff buffs = character.GetBuffs();
    while (buffs.BeNext()) {
        if (UTILITY_BUFFS.find(buffs.GetEffectType()) != UTILITY_BUFFS.end()) {
            return true;
        }
    }
    return false;
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

void SquadMember::upped() {
    if (uptimeTimer.is_stopped()) {
        uptimeTimer.start();
    }
}

void SquadMember::downed() {
    if (!uptimeTimer.is_stopped()) {
        updateUptime();
        uptimeTimer.stop();
    }
}

void SquadMember::updateUptime() {
    totalUptime += milliseconds(uptimeTimer.elapsed().wall / int(1e6));
    uptimeTimer.start();
}

void SquadMember::updateAlacrity(bool isActive) {
    if (alacrityTimer.is_stopped() && isActive) {
        alacrityTimer.start();
    }
    else if (!alacrityTimer.is_stopped() && !isActive) {
        alacrityDuration += milliseconds(alacrityTimer.elapsed().wall / int(1e6));
        alacrityTimer.stop();
    }
}
